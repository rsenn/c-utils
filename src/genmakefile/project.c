#include "project.h"
#include <unistd.h>
#include <sys/wait.h>
#include "genmakefile.h"
#include "compiler.h"
#include "generate.h"
#include "matrix.h"
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
  strarray tested; /* names asked about with #ifdef, #ifndef, defined(), never defined */
  strarray valued; /* names used by value in an #if or #elif, never defined */
  strarray cmp;    /* of those, the names used as more than a truth value */
  strarray defd;   /* the names of all #defines read */
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
strarray project_tested, project_valued;
const char* project_switch_prefix = "USE_";
strarray project_dep_names;
const char* project_libname = 0; /* --library: the sources of the arguments make one library */
strarray project_headers;        /* --header: public headers to install */ /* --dep: packages named on the command line */
static strarray project_cmp, project_defd;

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

static int is_module(const char* name);

/* a public header without sources of its own, lib/map.h: it stands for the
   headers it includes */
static int
is_facade(const char* path) {
  size_t n = str_len(libdir) + 1;
  char* name = str_ndup(path + n, str_len(path) - n - 2);
  int ret = !is_module(name);

  alloc_free(name);
  return ret;
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
  pp.track_queries = 1;

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

  /* a name the unit defined itself later on, an include guard for one, is no switch */
  strarray_foreach(&pp.undef_tested, p) if(!hashmap_get(&pp.macros, *p)) strarray_push_unique(&u->tested, *p);
  strarray_foreach(&pp.undef_valued, p) if(!hashmap_get(&pp.macros, *p)) strarray_push_unique(&u->valued, *p);
  strarray_foreach(&pp.undef_cmp, p) strarray_push_unique(&u->cmp, *p);
  strarray_foreach(&pp.defined_names, p) strarray_push_unique(&u->defd, *p);

  for(i = 0; i < strarray_size(&pp.inc_array); ++i) {
    const char* path = strarray_AT(&pp.inc_array, i);
    const char* by = hashmap_get(&pp.inc_list, path);
    char* r = rel(path);

    strarray_push_unique(&u->deps, r);

    /* a header named by the unit itself or by a header outside of lib/<name>.h:
       what the unit really uses, not what a library header pulls in;
       the sources of a module itself need all of them, as the types
       a header gets from another one (io.h and iarray.h) are used there */
    if(by && ((u->libname && is_lib_public(r)) || !str_diff(by, u->src) || !is_lib_public(rel(by)) || is_facade(rel(by)))) {
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
   "D" every header reached, "H" a project header, "L" a lib module, "Q" a name
   tested for being defined, "V" a name used by value, neither ever defined,
   "C" of the latter one used as more than a truth value, "F" the name of a
   #define */
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
    strarray_foreach(&u->tested, p) buffer_putm_internal(&b, "Q", *p, "\n", 0);
    strarray_foreach(&u->valued, p) buffer_putm_internal(&b, "V", *p, "\n", 0);
    strarray_foreach(&u->cmp, p) buffer_putm_internal(&b, "C", *p, "\n", 0);
    strarray_foreach(&u->defd, p) buffer_putm_internal(&b, "F", *p, "\n", 0);
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

      strarray_push_unique(line.s[0] == 'D'   ? &u->deps
                           : line.s[0] == 'H' ? &u->hdrs
                           : line.s[0] == 'Q' ? &u->tested
                           : line.s[0] == 'V' ? &u->valued
                           : line.s[0] == 'C' ? &u->cmp
                           : line.s[0] == 'F' ? &u->defd
                                              : &u->libs,
                           line.s + 1);
      stralloc_zero(&line);
    }

    stralloc_free(&line);
    close(fds[0]);
    waitpid(pid, &status, 0);
  }
#endif
}

static void
add_source(const char* s) {
  unit* u;

  if(!str_end(s, ".c") || path_is_directory(s))
    return;

  if(str_find(s, "/.") < str_len(s) || str_find(s, "third_party/") < str_len(s))
    return;

  u = alloc_zero(sizeof(unit));
  u->src = rel(s);

  if(hashmap_get(&unit_map, u->src))
    return;

  u->base = str_ndup(u->src, str_len(u->src) - 2);
  u->has_main = main_present(u->src) == 1;

  if(in_dir(u->src, libdir)) {
    const char* m = u->src + str_len(libdir) + 1;

    u->libname = str_ndup(m, str_chr(m, '/'));
  } else if(project_libname && !u->has_main) {
    u->libname = str_dup(project_libname);
  }

  hashmap_put(&unit_map, u->src, u);
  alloc_re(&units, n_units * sizeof(unit*), (n_units + 1) * sizeof(unit*));
  units[n_units++] = u;
}

/* a directory is walked, a .c file is taken as it is */
static void
collect(const char* dir) {
  rdir_t rd;
  char* s;

  if(!path_is_directory(dir)) {
    add_source(dir);
    return;
  }

  if(rdir_open(&rd, dir))
    return;

  while((s = rdir_read(&rd)))
    add_source(s);

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

  if(project_libname && !str_diff(name, project_libname))
    return 1;

  stralloc_init(&h);
  stralloc_catm(&h, libdir, "/", name, ".h");
  stralloc_nul(&h);
  ret = path_exists(h.s);

  /* a header alone, lib/windoze.h, makes no library: it needs sources in lib/<name>/ */
  if(ret) {
    stralloc_copys(&h, libdir);
    stralloc_catm(&h, "/", name);
    stralloc_nul(&h);
    ret = path_is_directory(h.s);
  }

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
  path_output2_dir(rel, out, ext, pathsep_args, bin ? &dirs.bin.sa : &dirs.obj.sa);
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

  /* the name is spelled with the separator of the make program, which need not be that of this system */
  stralloc_init(&dir);
  stralloc_copyb(&dir, name->s, str_rchrs(name->s, "/\\", 2) < name->len ? str_rchrs(name->s, "/\\", 2) : 0);
  stralloc_nul(&dir);

  if(dir.len && str_diff(dir.s, ".") && !rule_find_sa(&dir)) {
    generate_mkdir_rule(&dir);
    set_adds(&all->prereq, dir.s);
  }

  stralloc_free(&dir);
}

/* the object of a unit; with a program name it is one of the objects of that
   program, in a directory of its own below the object directory */
static void
obj_name(stralloc* out, unit* u, const char* prog) {
  if(prog) {
    stralloc in;

    stralloc_init(&in);
    stralloc_catm(&in, prog, "/", str_basename(u->src));
    stralloc_nul(&in);
    out_name(out, in.s, exts.obj, 0);
    stralloc_free(&in);
  } else {
    out_name(out, u->src, exts.obj, 0);
  }
}

static target*
compile_rule(unit* u, target* all, const char* prog) {
  stralloc obj, p;
  target* rule;
  char** d;

  stralloc_init(&obj);
  stralloc_init(&p);
  obj_name(&obj, u, prog);

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
  stralloc id, progname;
  int matrix = matrix_active();

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
  stralloc_init(&id);
  stralloc_init(&progname);
  stralloc_copys(&progname, str_basename(u->base));
  stralloc_nul(&progname);

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

  if(project_libname)
    lib_closure(project_libname, &closure);

  {
    stralloc name;

    stralloc_init(&name);
    stralloc_copys(&name, str_basename(u->base));

    if(matrix) {
      matrix_build_id(&id);
      stralloc_catm(&name, "_", id.s);
    }

    stralloc_nul(&name);
    out_name(&bin, name.s, exts.bin, 1);
    stralloc_free(&name);
  }

  if((rule = rule_get_sa(&bin))) {
    dir_rule(all, &bin);

    for(j = 0; j < strarray_size(&objs); ++j) {
      unit* o = hashmap_get(&unit_map, strarray_AT(&objs, j));

      obj_name(&p, o, matrix ? progname.s : 0);
      set_adds(&rule->prereq, p.s);

      if(matrix)
        compile_rule(o, all, progname.s);
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

  stralloc_free(&progname);
  stralloc_free(&id);
  stralloc_free(&p);
  stralloc_free(&bin);
}

/* the names of project_tested and project_valued that a build can switch on:
   those that begin with project_switch_prefix, that no #define of the sources
   gives a value (a fallback `#ifndef USE_X / #define USE_X 1` is one), that
   no #if compares or calculates with, and that are no variable set anyway */
void
project_switches(strarray* out) {
  strarray* from[2];
  size_t i;
  char** p;

  from[0] = &project_tested;
  from[1] = &project_valued;

  for(i = 0; i < 2; ++i)
    strarray_foreach(from[i], p) {
      if(!str_start(*p, project_switch_prefix))
        continue;

      if(strarray_contains(&project_defd, *p))
        LOG(2, "no switch, defined: ", *p);
      else if(strarray_contains(&project_cmp, *p))
        LOG(2, "no switch, compared or calculated with: ", *p);
      else if(!var_isset(*p))
        strarray_push_unique(out, *p);
    }

  strarray_sort(out, 0);
}

/* packages the sources know: the name, the macro the sources query and the
   variable the user sets to enable it */
static const char* const known_deps[][3] = {
    {"brotli", "HAVE_BROTLI", "LIBBROTLI"},
    {"bz2", "HAVE_LIBBZ2", "LIBBZ2"},
    {"lzma", "HAVE_LIBLZMA", "LIBLZMA"},
    {"zlib", "HAVE_ZLIB", "LIBZ"},
};

/* the optional packages of the project as "MACRO=VARIABLE": those whose macro
   the sources query without ever defining it, and those named with --dep */
void
project_deps(strarray* out) {
  size_t i, j;
  char** p;

  for(i = 0; i < sizeof(known_deps) / sizeof(known_deps[0]); ++i) {
    int found = strarray_contains(&project_tested, known_deps[i][1]) || strarray_contains(&project_valued, known_deps[i][1]);

    strarray_foreach(&project_dep_names, p) if(!str_diff(*p, known_deps[i][0])) found = 1;

    if(found && !strarray_contains(&project_defd, known_deps[i][1])) {
      stralloc sa;

      stralloc_init(&sa);
      stralloc_catm(&sa, known_deps[i][1], "=", known_deps[i][2], NULL);
      stralloc_nul(&sa);
      strarray_push_unique(out, sa.s);
      stralloc_free(&sa);
    }
  }

  /* any other name: HAVE_<NAME> enabled by LIB<NAME> */
  strarray_foreach(&project_dep_names, p) {
    int known = 0;
    stralloc sa;

    for(j = 0; j < sizeof(known_deps) / sizeof(known_deps[0]); ++j)
      if(!str_diff(*p, known_deps[j][0]))
        known = 1;

    if(known)
      continue;

    stralloc_init(&sa);
    stralloc_catm(&sa, "HAVE_", *p, "=LIB", *p, NULL);
    stralloc_nul(&sa);
    byte_upper(sa.s, sa.len);
    strarray_push_unique(out, sa.s);
    stralloc_free(&sa);
  }

  strarray_sort(out, 0);
}

/* where a header goes below include/: the part after an include directory
   of the sources, or the name of one given with --header; NULL for others */
static const char*
header_dest(const char* path) {
  size_t n = str_find(path, "/include/");
  char** h;

  if(n < str_len(path))
    return path + n + 9;

  if(str_start(path, "include/"))
    return path + 8;

  strarray_foreach(&project_headers, h) if(!str_diff(*h, str_basename(path))) return str_basename(path);

  return 0;
}

static void
install_cmd(stralloc* r, const char* mkdir_dir, const char* from, const char* to) {
  int win = pathsep_make == '\\';
  stralloc a, b;

  stralloc_init(&a);
  stralloc_init(&b);

  if(mkdir_dir) {
    stralloc_copys(&a, mkdir_dir);
    stralloc_nul(&a);
    stralloc_replacec(&a, win ? '/' : '\\', pathsep_make);
    stralloc_catm(r, newline, win ? "\tIF NOT EXIST " : "\tmkdir -p ", a.s, win ? " MKDIR " : "", win ? a.s : "", NULL);
  } else {
    stralloc_copys(&a, from);
    stralloc_nul(&a);
    stralloc_replacec(&a, win ? '/' : '\\', pathsep_make);
    stralloc_copys(&b, to);
    stralloc_nul(&b);
    stralloc_replacec(&b, win ? '/' : '\\', pathsep_make);
    stralloc_catm(r, newline, win ? "\tCOPY " : "\tcp ", a.s, " ", b.s, NULL);
  }

  stralloc_free(&a);
  stralloc_free(&b);
}

/* install: the library and its public headers below $(prefix) */
static void
install_rules(void) {
  target* inst = rule_get("install");
  strarray dirs;
  size_t i;
  char** d;
  stralloc libfile, dst, dir, hdrs;

  strarray_init(&dirs);
  stralloc_init(&libfile);
  stralloc_init(&dst);
  stralloc_init(&dir);
  stralloc_init(&hdrs);

  if(!var_isset("prefix")) {
    stralloc_copys(&dir, "inst-");
    stralloc_cats(&dir, tools.compiler);
    stralloc_nul(&dir);
    var_set("prefix", dir.s);
  }

  stralloc_init(&inst->recipe);
  set_adds(&inst->prereq, "all");
  strarray_push(&dirs, "$(prefix)/include");

  install_cmd(&inst->recipe, "$(prefix)", 0, 0);
  install_cmd(&inst->recipe, "$(prefix)/lib", 0, 0);
  install_cmd(&inst->recipe, "$(prefix)/include", 0, 0);

  for(i = 0; i < strarray_size(&lib_names); ++i) {
    stralloc rel;

    stralloc_init(&rel);
    stralloc_catm(&rel, "lib/", strarray_AT(&lib_names, i), NULL);
    stralloc_nul(&rel);
    out_name(&libfile, rel.s, exts.lib, 0);
    install_cmd(&inst->recipe, 0, libfile.s, "$(prefix)/lib");
    stralloc_free(&rel);
  }

  for(i = 0; i < n_units; ++i) {
    unit* u = units[i];

    if(!u->libname || (project_libname && str_diff(u->libname, project_libname)))
      continue;

    strarray_foreach(&u->deps, d) {
      const char* dest = header_dest(*d);

      if(!dest || strarray_contains(&dirs, *d))
        continue;

      strarray_push(&dirs, *d);
      stralloc_copys(&dst, "$(prefix)/include/");
      stralloc_cats(&dst, dest);
      stralloc_nul(&dst);

      {
        size_t cut = str_rchr(dst.s, '/');

        stralloc_copyb(&dir, dst.s, cut);
        stralloc_nul(&dir);
      }

      if(!strarray_contains(&dirs, dir.s)) {
        strarray_push(&dirs, str_dup(dir.s));
        install_cmd(&inst->recipe, dir.s, 0, 0);
      }

      install_cmd(&inst->recipe, 0, *d, dir.s);
    }
  }

  stralloc_free(&hdrs);
  stralloc_free(&dir);
  stralloc_free(&dst);
  stralloc_free(&libfile);
  strarray_free(&dirs);
}

int
project_generate(strarray* args, strarray* includes, int bins, int libs) {
  target* all = rule_get("all");
  size_t i;
  char **p, **l;

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

  {
    char num[FMT_ULONG];

    LOG(1, "found ", LOG_NUM(num, n_units), " sources, scanning them");
  }

  for(i = 0; i < n_units; ++i) {
    unit* u = units[i];

    LOG(2, "scanning ", u->src);
    scan_unit(u, includes);

    strarray_foreach(&u->tested, l) strarray_push_unique(&project_tested, *l);
    strarray_foreach(&u->valued, l) strarray_push_unique(&project_valued, *l);
    strarray_foreach(&u->cmp, l) strarray_push_unique(&project_cmp, *l);
    strarray_foreach(&u->defd, l) strarray_push_unique(&project_defd, *l);

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

  {
    char nt[FMT_ULONG], nv[FMT_ULONG];

    LOG(1, "found ", LOG_NUM(nt, strarray_size(&project_tested)), " names tested for being defined and ",
        LOG_NUM(nv, strarray_size(&project_valued)), " used by value, none of them defined");
    strarray_foreach(&project_tested, p) LOG(2, "tested: ", *p);
    strarray_foreach(&project_valued, p) LOG(2, "valued: ", *p);
  }

  {
    char nl[FMT_ULONG], np[FMT_ULONG];
    size_t progs = 0;

    for(i = 0; i < n_units; ++i)
      if(units[i]->has_main && !units[i]->libname)
        ++progs;

    LOG(1, "creating the rules for ", LOG_NUM(np, progs), " programs and ", LOG_NUM(nl, strarray_size(&lib_names)),
        " lib modules");
  }

  for(i = 0; i < n_units; ++i) {
    unit* u = units[i];

    if(u->libname && !is_module(u->libname))
      continue;

    /* the objects of a matrix point belong to the program that links them */
    if(!matrix_active())
      compile_rule(u, all, 0);
  }

  if(libs)
    for(i = 0; i < strarray_size(&lib_names); ++i)
      lib_rule(strarray_AT(&lib_names, i), all);

  if(bins)
    for(i = 0; i < n_units; ++i)
      if(units[i]->has_main && !units[i]->libname)
        program_rule(units[i], all);

  if(inst_libs || inst_bins)
    install_rules();

  return 0;
}
