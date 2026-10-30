#include "project.h"
#include <unistd.h>
#include <sys/wait.h>
#include "genmakefile.h"
#include "compiler.h"
#include "generate.h"
#include "path.h"
#include "rule.h"
#include "../../lib/alloc.h"
#include "../../lib/buffer.h"
#include "../../lib/byte.h"
#include "../../lib/cpp.h"
#include "../../lib/cpp_internal.h"
#include "../../lib/hashmap.h"
#include "../../lib/path.h"
#include "../../lib/rdir.h"
#include "../../lib/str.h"
#include "../../lib/stralloc.h"
#include "../../lib/strarray.h"

int main_present(const char*);

/* a .c file, with what its preprocessor run (platform defines applied)
   found out */
typedef struct {
  char* src;
  char* base;      /* src without ".c" */
  char* libname;   /* module name when below the lib directory, else NULL */
  int has_main;
  strarray deps;   /* every header reached, existing files only */
  strarray hdrs;   /* project headers named by the unit or by project headers */
  strarray libs;   /* lib modules named like that */
} unit;

static unit** units;
static size_t n_units;
static hashmap unit_map;   /* src -> unit* */
static hashmap lib_map;    /* module name -> strarray* of the libs it needs */
static strarray lib_names;
static const char* libdir = "lib";
static const char* const* predefs;
static stralloc cwd;

const char* project_libdir = "lib";
strarray project_defs;

/* path relative to the working directory */
static char*
rel(const char* p) {
  size_t n = str_len(p);

  if(cwd.len && n > cwd.len && byte_equal(p, cwd.len, cwd.s) && p[cwd.len] == '/')
    p += cwd.len + 1;

  while(p[0] == '.' && p[1] == '/')
    p += 2;

  return str_dup(p);
}

static int
in_dir(const char* path, const char* dir) {
  size_t n = str_len(dir);

  return str_start(path, dir) && path[n] == '/';
}

static int
is_lib_public(const char* path) {
  /* directly in the lib directory, as lib/<name>.h */
  size_t n = str_len(libdir);

  return in_dir(path, libdir) && str_chr(path + n + 1, '/') == str_len(path + n + 1) && str_end(path, ".h");
}

static void
define_one(cpp_ctx* pp, const char* d) {
  size_t n = str_chr(d, '=');
  char* name = str_ndup(d, n);

  cpp_define(pp, name, d[n] ? (char*)d + n + 1 : "");
}

static void
scan_unit_inproc(unit* u, strarray* incs) {
  cpp_ctx pp;
  cpp_token* tok;
  char** p;
  size_t i;

  cpp_init(&pp);
  pp.scan_only = 1;

  /* directories outside of the working directory belong to the compiler:
     their headers are not read, which would make a Windows build parse
     windows.h again for every unit */
  strarray_foreach(incs, p) {
    strarray_push(&pp.inc_paths, *p);

    if((*p)[0] == '/' && !(cwd.len && str_start(*p, cwd.s)))
      strarray_push(&pp.inc_system, *p);
  }

  strarray_push(&pp.inc_paths, libdir);

  if(!(tok = cpp_tokenize_file(&pp, u->src))) {
    cpp_free(&pp);
    return;
  }

  pp.base_file_name = u->src;
  cpp_init_macros(&pp);

  if(predefs)
    for(i = 0; predefs[i]; ++i)
      define_one(&pp, predefs[i]);

  strarray_foreach(&project_defs, p) define_one(&pp, *p);

  cpp_preprocess(&pp, tok);

  for(i = 0; i < strarray_size(&pp.inc_array); ++i) {
    const char* path = strarray_AT(&pp.inc_array, i);
    const char* by = hashmap_get(&pp.inc_list, path);
    char* r = rel(path);

    strarray_push_unique(&u->deps, r);

    /* a header named by the unit itself or by a header outside of lib/<name>.h:
       what the unit really uses, not what a library header pulls in */
    if(by && (!str_diff(by, u->src) || !is_lib_public(rel(by)))) {
      if(is_lib_public(r)) {
        char* name = str_ndup(r + str_len(libdir) + 1, str_len(r) - str_len(libdir) - 3);

        strarray_push_unique(&u->libs, name);
      } else
        strarray_push_unique(&u->hdrs, r);
    }
  }

  cpp_free(&pp);
}

/* the preprocessor never frees its tokens, so every unit is scanned by a
   child process that hands the result back over a pipe, one line per entry:
   "D" every header reached, "H" a project header, "L" a lib module */
static void
scan_unit(unit* u, strarray* incs) {
#if WINDOWS_NATIVE
  scan_unit_inproc(u, incs);
#else
  int fds[2];
  pid_t pid;

  if(pipe(fds) == -1 || (pid = fork()) == -1) {
    scan_unit_inproc(u, incs);
    return;
  }

  if(pid == 0) {
    buffer b;
    char buf[4096];
    char** p;

    close(fds[0]);
    scan_unit_inproc(u, incs);
    buffer_init(&b, (buffer_op_proto*)&write, fds[1], buf, sizeof(buf));

    strarray_foreach(&u->deps, p) buffer_putm_internal(&b, "D", *p, "\n", 0);
    strarray_foreach(&u->hdrs, p) buffer_putm_internal(&b, "H", *p, "\n", 0);
    strarray_foreach(&u->libs, p) buffer_putm_internal(&b, "L", *p, "\n", 0);
    buffer_flush(&b);
    _exit(0);
  } else {
    buffer b;
    char buf[4096];
    stralloc line;
    int status;

    close(fds[1]);
    buffer_init(&b, (buffer_op_proto*)&read, fds[0], buf, sizeof(buf));
    stralloc_init(&line);

    while(buffer_getline_sa(&b, &line) > 0) {
      stralloc_trimr(&line, "\n", 1);
      stralloc_nul(&line);

      if(line.len < 2)
        continue;

      strarray_push_unique(line.s[0] == 'D' ? &u->deps : line.s[0] == 'H' ? &u->hdrs : &u->libs, line.s + 1);
      stralloc_zero(&line);
    }

    stralloc_free(&line);
    close(fds[0]);
    waitpid(pid, &status, 0);
  }
#endif
}

static void
collect(const char* dir) {
  rdir_t rd;
  char* s;

  if(rdir_open(&rd, dir))
    return;

  while((s = rdir_read(&rd))) {
    unit* u;

    if(!str_end(s, ".c") || path_is_directory(s))
      continue;

    if(str_find(s, "/.") < str_len(s) || str_find(s, "third_party/") < str_len(s))
      continue;

    u = alloc_zero(sizeof(unit));
    u->src = rel(s);

    if(hashmap_get(&unit_map, u->src))
      continue;

    u->base = str_ndup(u->src, str_len(u->src) - 2);
    u->has_main = main_present(u->src) == 1;

    if(in_dir(u->src, libdir)) {
      const char* m = u->src + str_len(libdir) + 1;

      u->libname = str_ndup(m, str_chr(m, '/'));
    }

    hashmap_put(&unit_map, u->src, u);
    alloc_re(&units, n_units * sizeof(unit*), (n_units + 1) * sizeof(unit*));
    units[n_units++] = u;
  }

  rdir_close(&rd);
}

static unit*
unit_at(size_t i) {
  return units[i];
}

/* a library module has lib/<name>.h and sources in lib/<name>/ */
static int
is_module(const char* name) {
  stralloc h;
  int ret;

  stralloc_init(&h);
  stralloc_catm(&h, libdir, "/", name, ".h");
  stralloc_nul(&h);
  ret = path_exists(h.s);
  stralloc_free(&h);

  return ret;
}

static void
lib_closure(const char* name, strarray* out) {
  strarray* needs;
  char** p;

  if(strarray_contains(out, name))
    return;

  strarray_push(out, name);

  if((needs = hashmap_get(&lib_map, name)))
    strarray_foreach(needs, p) lib_closure(*p, out);
}


/* output name of rel (a source path) below the object or binary directory */
static void
out_name(stralloc* out, const char* rel, const char* ext, int bin) {
  stralloc_zero(out);
  (bin ? path_output2_bin : path_output2)(rel, out, ext, pathsep_args);
  stralloc_replacec(out, pathsep_args == '/' ? '\\' : '/', pathsep_args);
  stralloc_nul(out);
}

/* a path as the generated makefile spells it */
static void
make_path(stralloc* out, const char* p) {
  stralloc_copys(out, p);
  stralloc_replacec(out, pathsep_make == '/' ? '\\' : '/', pathsep_make);
  stralloc_nul(out);
}

/* mkdir rule for the directory of an output, made before anything else */
static void
dir_rule(target* all, const stralloc* name) {
  stralloc dir;

  stralloc_init(&dir);
  path_dirname_s(name->s, &dir);
  stralloc_nul(&dir);

  if(dir.len && str_diff(dir.s, ".") && !rule_find_sa(&dir)) {
    generate_mkdir_rule(&dir);
    set_adds(&all->prereq, dir.s);
  }

  stralloc_free(&dir);
}

static target*
compile_rule(unit* u, target* all) {
  stralloc obj, p;
  target* rule;
  char** d;

  stralloc_init(&obj);
  stralloc_init(&p);
  out_name(&obj, u->src, exts.obj, 0);

  if(!(rule = rule_get_sa(&obj)))
    return 0;

  dir_rule(all, &obj);
  make_path(&p, u->src);
  set_adds(&rule->prereq, p.s);

  strarray_foreach(&u->deps, d) {
    if(!str_diff(*d, u->src))
      continue;

    make_path(&p, *d);
    set_adds(&rule->prereq, p.s);
  }

  stralloc_weak(&rule->recipe, &commands.compile);
  stralloc_free(&p);
  stralloc_free(&obj);
  return rule;
}

static void
lib_rule(const char* name, target* all) {
  stralloc lib, rel, p;
  target* rule;
  size_t j;

  stralloc_init(&lib);
  stralloc_init(&rel);
  stralloc_init(&p);
  stralloc_catm(&rel, "lib/", name);
  stralloc_nul(&rel);
  out_name(&lib, rel.s, exts.lib, 0);

  if((rule = rule_get_sa(&lib))) {
    dir_rule(all, &lib);

    for(j = 0; j < n_units; ++j) {
      unit* u = units[j];

      if(u->libname && !str_diff(u->libname, name)) {
        out_name(&p, u->src, exts.obj, 0);
        set_adds(&rule->prereq, p.s);
      }
    }

    stralloc_weak(&rule->recipe, &commands.lib);
    set_adds(&all->prereq, lib.s);
  }

  stralloc_free(&p);
  stralloc_free(&rel);
  stralloc_free(&lib);
}

static void
program_rule(unit* u, target* all) {
  strarray objs, wanted, closure;
  stralloc bin, p;
  target* rule;
  size_t j;
  char** h;

  strarray_init(&objs);
  strarray_init(&wanted);
  strarray_init(&closure);
  stralloc_init(&bin);
  stralloc_init(&p);

  strarray_push(&objs, u->src);

  /* companion sources: foo.h named by the program, foo.c next to it without a main() */
  for(j = 0; j < strarray_size(&objs); ++j) {
    unit* o = hashmap_get(&unit_map, strarray_AT(&objs, j));

    if(!o)
      continue;

    strarray_foreach(&o->libs, h) strarray_push_unique(&wanted, *h);

    strarray_foreach(&o->hdrs, h) {
      stralloc c;
      unit* cu;

      stralloc_init(&c);
      stralloc_copyb(&c, *h, str_len(*h) - 2);
      stralloc_cats(&c, ".c");
      stralloc_nul(&c);

      if((cu = hashmap_get(&unit_map, c.s)) && !cu->has_main && !cu->libname)
        strarray_push_unique(&objs, cu->src);

      stralloc_free(&c);
    }
  }

  strarray_foreach(&wanted, h) if(is_module(*h)) lib_closure(*h, &closure);

  {
    stralloc name;

    stralloc_init(&name);
    stralloc_copys(&name, str_basename(u->base));
    stralloc_nul(&name);
    out_name(&bin, name.s, exts.bin, 1);
    stralloc_free(&name);
  }

  if((rule = rule_get_sa(&bin))) {
    dir_rule(all, &bin);

    for(j = 0; j < strarray_size(&objs); ++j) {
      unit* o = hashmap_get(&unit_map, strarray_AT(&objs, j));

      out_name(&p, o->src, exts.obj, 0);
      set_adds(&rule->prereq, p.s);
    }

    strarray_foreach(&closure, h) {
      stralloc rel;

      stralloc_init(&rel);
      stralloc_catm(&rel, "lib/", *h);
      stralloc_nul(&rel);
      out_name(&p, rel.s, exts.lib, 0);
      set_adds(&rule->prereq, p.s);
      stralloc_free(&rel);
    }

    stralloc_weak(&rule->recipe, &commands.link);
    set_adds(&all->prereq, bin.s);
  }

  stralloc_free(&p);
  stralloc_free(&bin);
}

int
project_generate(strarray* args, strarray* includes, int bins, int libs) {
  target* all = rule_get("all");
  size_t i;
  char** p;

  units = 0;
  n_units = 0;
  byte_zero(&unit_map, sizeof(unit_map));
  byte_zero(&lib_map, sizeof(lib_map));
  hashmap_init(&unit_map);
  hashmap_init(&lib_map);
  strarray_init(&lib_names);
  stralloc_init(&cwd);
  path_getcwd(&cwd);
  stralloc_nul(&cwd);

  libdir = project_libdir;
  predefs = compiler_predefs(tools.compiler);

  strarray_foreach(args, p) collect(*p);

  /* the lib modules are needed wherever the programs are */
  if(path_is_directory(libdir))
    collect(libdir);

  for(i = 0; i < n_units; ++i) {
    unit* u = units[i];

    scan_unit(u, includes);

    if(u->libname && is_module(u->libname)) {
      strarray* needs = hashmap_get(&lib_map, u->libname);
      char** l;

      if(!needs) {
        needs = alloc_zero(sizeof(strarray));
        hashmap_put(&lib_map, u->libname, needs);
        strarray_push_unique(&lib_names, u->libname);
      }

      strarray_foreach(&u->libs, l) if(str_diff(*l, u->libname) && is_module(*l)) strarray_push_unique(needs, *l);
    }
  }

  for(i = 0; i < n_units; ++i) {
    unit* u = units[i];

    if(u->libname && !is_module(u->libname))
      continue;

    compile_rule(u, all);
  }

  if(libs)
    for(i = 0; i < strarray_size(&lib_names); ++i)
      lib_rule(strarray_AT(&lib_names, i), all);

  if(bins)
    for(i = 0; i < n_units; ++i)
      if(units[i]->has_main && !units[i]->libname)
        program_rule(units[i], all);

  return 0;
}
