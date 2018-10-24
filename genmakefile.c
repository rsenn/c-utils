#include "lib/buffer.h"
#include "lib/getopt.h"
#include "lib/hmap.h"
#include "lib/mmap.h"
#include "lib/open.h"
#include "lib/path.h"
#include "lib/rdir.h"
#include "lib/scan.h"
#include "lib/slist.h"
#include "lib/str.h"
#include "lib/stralloc.h"
#include "lib/strarray.h"
#include "lib/strlist.h"
#include "lib/windoze.h"

#include <ctype.h>

#if WINDOWS
#define MAX_CMD_LEN 1023

#define DEFAULT_OBJEXT ".obj"
#define DEFAULT_LIBEXT ".lib"
#define DEFAULT_EXEEXT ".exe"
#else
#define MAX_CMD_LEN 8191

#define DEFAULT_OBJEXT ".o"
#define DEFAULT_LIBEXT ".a"
#define DEFAULT_EXEEXT ""
#endif

#if WINDOWS_NATIVE
#define DEFAULT_PATHSEP '\\'
#else
#define DEFAULT_PATHSEP '/'
#endif

typedef struct {
  enum { X86, ARM } arch;
  enum { _32, _64 } bits;
} machine_type;

typedef struct {
  struct slink link;
  const char* name;
} sourcefile;

typedef struct {
  int n_sources;
  slink* sources;
  strlist includes;
} sourcedir;

typedef struct {
  const char* name;
  strlist prereq;
  stralloc* recipe;
  array deps;
} target;

enum {
  BUILD_TYPE_RELEASE = 0,
  BUILD_TYPE_RELWITHDEBINFO,
  BUILD_TYPE_MINSIZEREL,
  BUILD_TYPE_DEBUG,
};

const char* const build_types[] = {"Release", "RelWithDebInfo", "MinSizeRel", "Debug"};

typedef void(linklib_fmt)(const char*, stralloc*);

static strarray srcs;
static stralloc compile_command, lib_command, link_command, mkdir_command, delete_command;
static const char* objext = DEFAULT_OBJEXT;
static const char* libext = DEFAULT_LIBEXT;
static const char* binext = DEFAULT_EXEEXT;
static strlist builddir, outdir;
static stralloc srcdir;
static char pathsep = DEFAULT_PATHSEP;
static int build_type = BUILD_TYPE_DEBUG;

static HMAP_DB *sourcedirs, *rules, *vars;

static machine_type mach;

static linklib_fmt* format_linklib_fn;

void
debug_sa(const char* name, stralloc* sa) {
  buffer_puts(buffer_2, name);
  buffer_puts(buffer_2, ": ");
  buffer_putsa(buffer_2, sa);
  buffer_putnlflush(buffer_2);
}

void
debug_s(const char* name, const char* s) {
  buffer_puts(buffer_2, name);
  buffer_puts(buffer_2, ": ");
  buffer_puts(buffer_2, s);
  buffer_putnlflush(buffer_2);
}

void
path_prefix_s(const stralloc* prefix, const char* path, stralloc* out) {
  stralloc_zero(out);
  if(prefix->len) {
    stralloc_cat(out, prefix);
    stralloc_catc(out, pathsep);
  }
  stralloc_cats(out, path);
}

char*
path_wildcard(const char* path, stralloc* sa) {
  const char* x;
  size_t n, e;
  stralloc_copys(sa, path);
  stralloc_nul(sa);
  x = path_basename(sa->s);
  x = path_skip_separator(x);
  n = x - sa->s;
  e = byte_rchr(x, sa->len - n, '.');

  stralloc_remove(sa, n, e);
  stralloc_insertb(sa, "*", n, 1);
  stralloc_nul(sa);
  return sa->s;
}

/**
 * Output library name (+".lib")
 */
void
format_linklib_lib(const char* libname, stralloc* out) {
  stralloc_cats(out, libname);
  stralloc_cats(out, libext);
}

/**
 * Output library name (+ leading "-l")
 */
void
format_linklib_switch(const char* libname, stralloc* out) {
  stralloc_cats(out, "-l");
  stralloc_cats(out, libname);

  if(stralloc_endb(out, "lib", 3)) out->len -= 3;
}

/**
 * Checks if the given source file contains a main() function
 */
int
scan_main(const char* x, size_t n) {
  while(n) {
    size_t i = byte_finds(x, n, "main");
    if(i + 5 >= n) return 0;
    i += 4;
    x += i;
    n -= i;
    if(i > 4 && !isspace(*(x - 5))) continue;
    if((i = scan_whitenskip(x, n)) == n) break;
    x += i;
    n -= i;
    if(*x == '(') return 1;
  }
  return 0;
}

int
has_main(const char* filename) {
  char* x;
  size_t n;
  if((x = mmap_read(filename, &n))) {
    int ret = scan_main(x, n);
    mmap_unmap(x, n);
    return ret;
  }
  return -1;
}

/**
 * Extract #include directives
 */
void
extract_includes(const char* x, size_t n, strlist* includes, int sys) {
  while(n) {
    size_t i;
    if((i = scan_charsetnskip(x, " \t\r\n", n)) == n) break;
    x += i;
    n -= i;
    if(*x == '#') {
      x += 1;
      n -= 1;
      if((i = scan_charsetnskip(x, " \t\r", n) + 7) >= n) break;
      x += i;
      n -= i;
      if(!str_diffn(x - 7, "include", 7)) {
        char quote;
        if((i = scan_charsetnskip(x, " \t\r", n) + 1) >= n) break;
        x += i;
        n -= i;
        quote = *(x - 1);
        if((sys && quote == '<') || quote == '"') {
          char set[3];
          set[0] = (quote == '<' ? '>' : '"');
          set[1] = '\n';
          set[2] = '\0';
          if((i = scan_noncharsetnskip(x, set, n)) >= n) break;

          strlist_pushb_unique(includes, x, i);
          x += i + 1;
          n -= i + 1;
        }
      }
    }
    if((i = byte_chr(x, n, '\n')) >= n) break;
    x += i;
    n -= i;
  }
}

int
get_includes(const char* srcfile, strlist* includes, int sys) {
  const char* x;
  size_t n;

  if((x = mmap_read(srcfile, &n))) {
    extract_includes(x, n, includes, sys);
    return 1;
  }
  return 0;
}

/**
 * Get rule command with substitutions
 */
void
rule_command(target* rule, stralloc* out) {
  size_t i;
  stralloc* in = rule->recipe;

  for(i = 0; i < in->len; ++i) {
    const char* p = &in->s[i];

    if(i + 2 <= in->len && *p == '$' && str_chr("@^<", p[1]) < 3) {
      switch(p[1]) {
        case '@': {
          stralloc_cats(out, rule->name);
          break;
        }
        case '^': {
          stralloc_cat(out, &rule->prereq.sa);
          break;
        }
        case '<': {
          size_t n;
          const char* s = strlist_at_n(&rule->prereq, 0, &n);
          stralloc_catb(out, s, n);
          break;
        }
      }
      ++i;
    } else {
      if(!stralloc_append(out, p)) break;
    }
  }
}

/**
 * Find or create rule
 */
target*
get_rule(const char* name) {
  target* ret = NULL;
  TUPLE* t;

  if(rules == NULL) hmap_init(1024, &rules);

  if(hmap_search(rules, name, str_len(name) + 1, &t) == HMAP_SUCCESS) {
    ret = t->vals.val_custom;
  } else {
    ret = malloc(sizeof(target));
    byte_zero(ret, sizeof(target));

    hmap_add(&rules, name, str_len(name) + 1, 0, HMAP_DATA_TYPE_CUSTOM, ret);
    hmap_search(rules, name, str_len(name) + 1, &t);

    ret->name = t->key;

    strlist_init(&ret->prereq, ' ');
  }

  return ret;
}

target*
get_rule_sa(stralloc* name) {
  stralloc_nul(name);
  return get_rule(name->s);
}

target*
find_rule(const char* needle) {
  TUPLE* t;

  hmap_foreach(rules, t) {
    const char* name = t->key;
    if(str_equal(name, needle)) return t->vals.val_custom;
    if(str_equal(str_basename(name), str_basename(needle))) return t->vals.val_custom;
  }
  return 0;
}

target*
find_rule_sa(stralloc* name) {
  stralloc_nul(name);
  return find_rule(name->s);
}

target*
find_rule_b(const char* x, size_t n) {
  target* r;
  stralloc sa;
  stralloc_init(&sa);
  stralloc_copyb(&sa, x, n);
  r = find_rule_sa(&sa);
  stralloc_free(&sa);
  return r;
}

/**
 * Add a path to a strlist
 */
void
add_path(strlist* list, const char* path) {
  size_t i, len = str_len(path);

  strlist_push(list, path);

  for(i = list->sa.len - len; i < list->sa.len; ++i) {
    if(list->sa.s[i] == '/' || list->sa.s[i] == '\\') list->sa.s[i] = pathsep;
  }
}

void
add_srcpath(strlist* list, const char* path) {
  size_t i, len = str_len(path);

  if(srcdir.len) {
    strlist_push_sa(list, &srcdir);
    stralloc_cats(&list->sa, path);
  } else {
    strlist_push(list, path);
  }
}

void
add_path_sa(strlist* list, stralloc* path) {
  stralloc_nul(path);
  add_path(list, path->s);
}

/**
 * Find or create variable
 */
strlist*
get_var(const char* name) {
  TUPLE* t;

  if(vars == NULL) hmap_init(1024, &vars);

  if(hmap_search(vars, name, str_len(name) + 1, &t) != HMAP_SUCCESS) {
    strlist var;
    strlist_init(&var, ' ');

    hmap_set(&vars, name, str_len(name) + 1, &var, sizeof(strlist));
    hmap_search(vars, name, str_len(name) + 1, &t);
  }

  return (strlist*)t->vals.val_chars;
}

/**
 * Set variable
 */
void
set_var(const char* name, const char* value) {
  strlist* var = get_var(name);

  stralloc_zero(&var->sa);
  stralloc_copys(&var->sa, value);
}

/**
 * Add value to variable
 */
void
push_var(const char* name, const char* value) {
  strlist* var = get_var(name);

  strlist_push_unique(var, value);
}

void
push_var_sa(const char* name, stralloc* value) {
  strlist_push_unique_sa(get_var(name), value);
}

/**
 * Add library spec to variable
 */
void
push_lib(const char* name, const char* lib) {
  strlist* var = get_var(name);

  if(var->sa.len) stralloc_catc(&var->sa, var->sep);

  format_linklib_fn(lib, &var->sa);
}

void
with_lib(const char* lib) {
  stralloc def;
  stralloc_init(&def);
  stralloc_copys(&def, "-DHAVE_");

  if(str_find(lib, "lib") == str_len(lib)) stralloc_cats(&def, "LIB");

  stralloc_cats(&def, lib);
  stralloc_cats(&def, "=1");
  byte_upper(def.s, def.len);

  push_var_sa("DEFS", &def);

  push_lib("LIBS", lib);
}

/**
 * Search rules by command
 */
void
get_rules_by_cmd(stralloc* cmd, strlist* deps) {
  TUPLE* t;

  hmap_foreach(rules, t) {
    target* rule = t->vals.val_custom;

    if(rule->recipe == cmd) {
      strlist_push(deps, rule->name);
    }
  }
}

/**
 * Create new source file entry.
 */
sourcefile*
new_source(const char* name) {
  sourcefile* ret;

  if((ret = malloc(sizeof(sourcefile)))) {
    byte_zero(ret, sizeof(sourcefile));
    ret->name = str_dup(name);
    return ret;
  }
  return 0;
}

/**
 * Change file extension and concatenate it to out.
 */
char*
change_ext(const char* in, stralloc* out, const char* ext) {
  size_t extpos = str_rchr(in, '.');

  stralloc_catb(out, in, extpos);
  stralloc_cats(out, ext);
  stralloc_nul(out);
  return out->s;
}

/**
 * Convert source file name to object file name
 */
char*
src_to_obj(const char* in, stralloc* out) {
  stralloc_zero(out);

  if(builddir.sa.len) {
    stralloc_cat(out, &builddir.sa);

    if(!stralloc_endb(out, &pathsep, 1)) stralloc_catc(out, pathsep);
  }

  return change_ext(str_basename(in), out, objext);
}

/**
 * Gets directory name from a file path (allocated).
 */
char*
dirname_alloc(const char* p) {
  size_t len = str_len(p);
  size_t pos = str_rchrs(p, "\\/", 2);

  if(pos < len) return str_ndup(p, pos);

  return str_dup(".");
}

/**
 * Adds a source file to the given list.
 */
void
add_source(const char* filename, strarray* sources) {
  if(str_end(filename, ".c")) {
    stralloc sa;
    stralloc_init(&sa);
    stralloc_copys(&sa, filename);
    //    stralloc_replace(&sa, pathsep == '/' ? '\\' : '/', pathsep);

    strarray_push_sa(sources, &sa);

    stralloc_free(&sa);
  }
}

/**
 * Searches all source files in the given directory and creates a string-array.
 */
void
get_sources(const char* basedir, strarray* sources) {
  rdir_t rdir;

  if(!rdir_open(&rdir, basedir)) {
    const char* s;

    while((s = rdir_read(&rdir))) {
      add_source(s, sources);
    }
  }
}

/**
 * Creates a hash-map of all source directories
 */
void
populate_sourcedirs(strarray* sources, HMAP_DB* sourcedirs) {
  char** srcfile;
  stralloc dir;
  stralloc_init(&dir);

  strarray_foreach(sources, srcfile) {
    size_t n;
    char* x;
    sourcedir* srcdir;
    sourcefile* file = new_source(*srcfile);

    path_dirname(*srcfile, &dir);

    if((srcdir = hmap_get(sourcedirs, dir.s, dir.len + 1))) {
      slist_add(&srcdir->sources, &file->link);

      ++srcdir->n_sources;
    } else {
      sourcedir newdir;
      byte_zero(&newdir, sizeof(newdir));

      newdir.n_sources = 1;
      newdir.sources = &file->link;
      strlist_init(&newdir.includes, '\0');

      hmap_set(&sourcedirs, dir.s, dir.len + 1, &newdir, sizeof(newdir));

      srcdir = hmap_get(sourcedirs, dir.s, dir.len + 1);
    }

    if((x = mmap_read(file->name, &n))) {
      const char* s;
      size_t dlen = dir.len;
      stralloc r;
      strlist l;
      stralloc_init(&r);
      strlist_init(&l, '\0');

      extract_includes(x, n, &l, 0);

      stralloc_replace(&l.sa, pathsep == '\\' ? '/' : '\\', pathsep);

      strlist_foreach_s(&l, s) {
        dir.len = dlen;
        stralloc_catc(&dir, pathsep);
        stralloc_cats(&dir, s);

        path_canonical_sa(&dir, &r);

        strlist_push_unique_sa(&srcdir->includes, &r);
      }

      stralloc_free(&r);
      strlist_free(&l);

      mmap_unmap(x, n);
    } else {
      buffer_putm_internal(buffer_2, "ERROR opening '", file->name, "'\n", 0);
      buffer_putnlflush(buffer_2);
    }
  }

  stralloc_free(&dir);
}

void
dump_sourcedirs(buffer* b, HMAP_DB* sourcedirs) {
  TUPLE* t;

  hmap_foreach(sourcedirs, t) {
    sourcedir* srcdir = hmap_data(t);
    sourcefile* pfile;

    buffer_puts(b, "source dir(");
    buffer_putulong(b, srcdir->n_sources);
    buffer_puts(b, "): ");
    buffer_put(b, t->key, t->key_len);

    slist_foreach(srcdir->sources, pfile) {
      buffer_putspace(b);
      buffer_puts(b, pfile->name);
    }

    buffer_putnlflush(b);
  }
}

/**
 * Generate compile rules for every source file given
 */
void
compile_rules(HMAP_DB* rules, strarray* sources) {
  char** srcfile;
  stralloc obj;
  stralloc_init(&obj);

  strarray_foreach(sources, srcfile) {
    target* rule;

    src_to_obj(*srcfile, &obj);

    if((rule = get_rule_sa(&obj))) {
      add_srcpath(&rule->prereq, *srcfile);

      rule->recipe = &compile_command;
    }
  }

  stralloc_free(&obj);
}

/**
 * Include list to library list
 */
void
includes_to_libs(const strlist* includes, strlist* libs) {
  const char* s;
  size_t n;
  stralloc sa;
  stralloc_init(&sa);
  stralloc_zero(&libs->sa);

  strlist_foreach(includes, s, n) {
    target* rule;
    stralloc_copyb(&sa, s, n);
    if(stralloc_endb(&sa, ".h", 2)) sa.len -= 2;
    if(stralloc_endb(&sa, "_internal", 9)) sa.len -= 9;

    stralloc_cats(&sa, libext);

    //    debug_sa("includes_to_libs", &sa);

    if((rule = find_rule_sa(&sa))) {
      strlist_push(libs, rule->name);
    }
  }
  stralloc_free(&sa);
}

/**
 * Given a list of target names, outputs an array of pointers to those targets.
 */
void
target_ptrs(const strlist* targets, array* out) {
  const char* x;
  size_t n;

  strlist_foreach(targets, x, n) {
    target* rule;

    if((rule = find_rule_b(x, n))) {
      if(!array_find(out, sizeof(target*), &rule)) array_catb(out, &rule, sizeof(target*));
    } else {
      buffer_puts(buffer_2, "ERROR: rule '");
      buffer_put(buffer_2, x, n);
      buffer_puts(buffer_2, "' not found");
      buffer_putnlflush(buffer_2);
    }
  }
}

void
target_deps_internal(buffer* b, target* t, strlist* all, int depth) {
  target** ptr;

  array_foreach_t(&t->deps, ptr) {
    if(strlist_push_unique(all, (*ptr)->name)) {
      buffer_puts(b, "# ");
      buffer_putnspace(b, depth * 2);
      buffer_puts(b, str_basename((*ptr)->name));
      buffer_putnlflush(b);

      target_deps_internal(b, *ptr, all, depth + 1);
    }
  }
}

void
target_deps(buffer* b, target* t) {
  strlist deplist;
  strlist_init(&deplist, '\0');
  strlist_push(&deplist, t->name);

  buffer_putm_internal(b, "# Dependencies for '", t->name, "':\n", 0);

  target_deps_internal(b, t, &deplist, 0);

  strlist_free(&deplist);
}

/**
 * Output rule to buffer
 */
void
output_rule(buffer* b, target* rule) {
  int num_deps = strlist_count(&rule->prereq);

  if(array_length(&rule->deps, sizeof(target*))) {
    target_deps(b, rule);
    /*    target** r;
        buffer_puts(b, "# Dependencies:\n");
        array_foreach_t(&rule->deps, r) {

          buffer_puts(b, "#  ");

          buffer_puts(b, (*r)->name);
          buffer_putnlflush(b);
        }*/
  }

  if(num_deps == 0 && str_diffn(rule->name, builddir.sa.s, builddir.sa.len)) {
    buffer_putm_internal(b, ".PHONY: ", rule->name, "\n", 0);
  }

  buffer_puts(b, rule->name);
  buffer_putc(b, ':');

  if(num_deps) {
    buffer_putspace(b);
    buffer_putsa(b, &rule->prereq.sa);
  }

  if(rule->recipe) {
    stralloc cmd;
    stralloc_init(&cmd);

    rule_command(rule, &cmd);

    buffer_puts(b, "\n\t");
    buffer_putsa(b, &cmd);
    buffer_putc(b, '\n');

    stralloc_free(&cmd);
  }

  buffer_putnlflush(b);
}

/**
 * Generate compile rules for every source file with a main()
 */
void
link_rules(HMAP_DB* rules, strarray* sources) {
  target* all;
  char** srcfile;
  strlist incs, deps;
  stralloc obj, bin;
  strlist_init(&incs, '\0');
  strlist_init(&deps, ' ');
  stralloc_init(&obj);
  stralloc_init(&bin);
  all = get_rule("all");

  strarray_foreach(sources, srcfile) {
    target *compile, *link;

    strlist_zero(&incs);
    strlist_zero(&deps);

    if(has_main(*srcfile) == 1) {

      src_to_obj(*srcfile, &obj);

      if((compile = find_rule_sa(&obj))) {

        get_includes(*srcfile, &incs, 0);
        includes_to_libs(&incs, &deps);
      }

      stralloc_zero(&bin);
      change_ext(obj.s, &bin, binext);

      add_path_sa(&all->prereq, &bin);

      if((link = get_rule_sa(&bin))) {
        add_path_sa(&link->prereq, &obj);

        get_rules_by_cmd(&lib_command, &link->prereq);

        link->recipe = &link_command;

#ifdef DEBUG_OUTPUT
        buffer_putm_internal(buffer_2, "Deps for '", link->name, "': ", 0);
        buffer_putsa(buffer_2, &deps.sa);
        buffer_putnlflush(buffer_2);
#endif

        target_ptrs(&deps, &link->deps);
      }
    }
  }

  stralloc_free(&bin);
  stralloc_free(&obj);
}

/**
 * Get source dir
 */
sourcedir*
get_srcdir(const char* path) {
  return hmap_get(sourcedirs, path, str_len(path) + 1);
}

sourcedir*
get_srcdir_sa(stralloc* path) {
  stralloc_nul(path);
  return hmap_get(sourcedirs, path->s, path->len + 1);
}

sourcedir*
get_srcdir_b(const char* x, size_t n) {
  sourcedir* ret;
  stralloc p;
  stralloc_init(&p);
  stralloc_copyb(&p, x, n);
  ret = get_srcdir_sa(&p);
  stralloc_free(&p);
  return ret;
}
void
path_prefix_b(const stralloc* prefix, const char* path, size_t n, stralloc* out) {
  stralloc_zero(out);
  if(prefix->len) {
    stralloc_cat(out, prefix);
    stralloc_catc(out, pathsep);
  }
  stralloc_catb(out, path, n);
}

/**
 * Generate lib rule for source dir
 */
target*
lib_rule_for_srcdir(HMAP_DB* rules, sourcedir* srcdir, const char* name) {
  target* rule;
  stralloc sa;
  stralloc_init(&sa);

  path_prefix_s(&builddir.sa, name, &sa);
  stralloc_cats(&sa, libext);

  //  debug_sa("lib_rule_for_srcdir", &sa);

  if((rule = get_rule_sa(&sa))) {
    sourcefile* pfile;
    strlist_init(&rule->prereq, ' ');

    slist_foreach(srcdir->sources, pfile) {
      src_to_obj(pfile->name, &sa);

      add_path_sa(&rule->prereq, &sa);
    }

    rule->recipe = &lib_command;
  }

  stralloc_free(&sa);
  return rule;
}

void
deps_for_libs(HMAP_DB* rules) {
  TUPLE* t;
  stralloc sa;
  stralloc_init(&sa);

  hmap_foreach(sourcedirs, t) {
    sourcedir* srcdir = hmap_data(t);
    target* rule;
    size_t n;
    const char* s;

    path_prefix_s(&builddir.sa, str_basename(t->key), &sa);
    stralloc_cats(&sa, libext);

    if((rule = find_rule_sa(&sa))) {
      strlist libs;
      strlist_init(&libs, '\0');

      includes_to_libs(&srcdir->includes, &libs);

      target_ptrs(&libs, &rule->deps);
    }
  }
  stralloc_free(&sa);
}

/**
 * Generate compile rules for every library given
 */
void
lib_rules(HMAP_DB* rules, HMAP_DB* srcdirs) {
  target* rule;
  TUPLE* t;
  stralloc inc;
  stralloc_init(&inc);

  hmap_foreach(srcdirs, t) {
    sourcedir* srcdir = hmap_data(t);
    const char *s, *base = str_basename(t->key);
    size_t n;

    if(str_equal(base, "lib") || base[0] == '.' || base[0] == '\0') continue;

    rule = lib_rule_for_srcdir(rules, srcdir, base);
  }
  stralloc_free(&inc);
}

void
clean_rule(HMAP_DB* rules) {
  target* rule;

  /* Generate "clean" rule */
  if((rule = get_rule("clean"))) {
    TUPLE* t;
    char* arg;
    int32 lineoffs = 0;
    stralloc fn;
    strlist delete_args;
    stralloc_init(&fn);
    strlist_init(&delete_args, '\0');

    stralloc_copys(&delete_command, "DEL /F /Q");

    hmap_foreach(rules, t) {

      /* Ignore the builddir rule */
      if(stralloc_equals(&builddir.sa, t->key)) continue;

      rule = hmap_data(t);

      /* If the rule has prerequisites and a recipe, it must be a producing rule */
      if(strlist_count(&rule->prereq) && rule->recipe) {

        /* If possible, transform file name into a wildcard pattern */
        arg = path_wildcard(t->key, &fn);

        /* Add to deletion list */
        strlist_push_unique(&delete_args, arg);
      }
    }

    strlist_foreach_s(&delete_args, arg) {

      if(delete_command.len - lineoffs + str_len(arg) >= MAX_CMD_LEN) {
        stralloc_cats(&delete_command, "\n\tDEL /F /Q");
        lineoffs = delete_command.len;
      }

      stralloc_catc(&delete_command, ' ');
      stralloc_cats(&delete_command, arg);

      if(arg[str_chr(arg, '*')]) lineoffs = -MAX_CMD_LEN;
    }

    rule->recipe = &delete_command;
  }
}

/**
 * Output all variables
 */
void
output_all_vars(buffer* b, HMAP_DB* vars) {
  TUPLE* t;

  hmap_foreach(vars, t) {
    strlist* var = hmap_data(t);

    buffer_puts(b, t->key);
    buffer_puts(b, " = ");
    buffer_putsa(b, &var->sa);
    buffer_putc(b, '\n');
  }
  buffer_putnlflush(b);
}

/**
 * Output the rule set
 */
void
output_all_rules(buffer* b, HMAP_DB* hmap) {
  TUPLE* t;

  hmap_foreach(hmap, t) { output_rule(b, t->vals.val_custom); }
}

void
strarray_dump(buffer* b, const strarray* arr) {
  char **p = strarray_begin(arr), **e = strarray_end(arr);

  while(p < e) {
    buffer_puts(b, *p);
    buffer_putnlflush(b);
    ++p;
  }
}

/**
 * Show command line usage
 */
void
usage(char* argv0) {
  buffer_putm_internal(buffer_1,
                       "Usage: ",
                       str_basename(argv0),
                       " [sources...]\n",
                       "\n",
                       "Options\n",
                       "  -h, --help                show this help\n",
                       "  -o, --output FILE         write to file\n"
                       "  -O, --objext EXT          object file extension\n",

                       "  -B, --exeext EXT          binary file extension\n",
                       "  -L, --libext EXT          library file extension\n",
                       "  -l, --create-libs         create rules for libraries\n",
                       "  -o, --create-objs         create rules for objects\n",
                       "  -b, --create-bins         create rules for programs\n",
                       "  -d, --builddir            build directory\n",
                       "  -t, --type   TYPE         makefile type, one of:\n"
                       "\n"
                       "     gcc         GNU make\n"
                       "     bcc55       Borland C++ Builder 5.5\n"
                       "     bcc32       Borland C++ Builder new\n"
                       "     lcc         lcc make\n"
                       "     tcc         Tinycc make\n"
                       "     msvc        Visual C++ NMake\n"
                       "     icl         Intel C++ NMake\n"
                       "     clang       LLVM NMake\n"
                       "\n",
                       0);
  buffer_putnlflush(buffer_1);
}

/**
 * Set the machine type
 */
int
set_machine(const char* s) {

  int ret = 1;

  if(s[str_find(s, "64")])
    mach.bits = _64;
  else if(s[str_find(s, "32")])
    mach.bits = _32;
  else if(s[str_find(s, "386")])
    mach.bits = _32;
  else
    ret = 0;

  if(s[str_find(s, "arm")] || s[str_find(s, "aarch")])
    mach.arch = ARM;
  else if(s[str_find(s, "86")])
    mach.arch = X86;
  else
    ret = 0;

  return ret;
}

/**
 * Set the compiler type
 */
int
set_compiler(const char* type) {

  push_var("CC", "cc");
  push_var("CXX", "c++");

  stralloc_copys(&compile_command, "$(CC) $(CFLAGS) $(CPPFLAGS) $(DEFS) -c -o $@ $<");

  push_var("CFLAGS", "-O2");
  push_var("DEFS", "-DHAVE_ERRNO_H=1");

  stralloc_copys(&mkdir_command, "IF NOT EXIST \"$@\" MKDIR \"$@\"");

  if(str_start(type, "gnu") || str_start(type, "gcc") || str_start(type, "clang") || str_start(type, "llvm")) {

    libext = ".a";
    objext = ".o";

    /*
     * GNU GCC compatible compilers
     */

    if(str_start(type, "gnu") || str_start(type, "gcc")) {
      set_var("CC", "gcc");
      set_var("CXX", "g++");

      set_var("AR", "ar");
    } else if(str_start(type, "clang") || str_start(type, "llvm")) {
      set_var("CC", "clang");
      set_var("CXX", "clang++");

      set_var("AR", "llvm-ar");
    }

    stralloc_copys(&lib_command, "$(AR) rcs $@ $^");
    stralloc_copys(&link_command, "$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS) $(EXTRA_LIBS)");

    format_linklib_fn = &format_linklib_switch;

    /*
     * Visual C++ compiler
     */
  } else if(str_start(type, "msvc") || str_start(type, "icl")) {

    pathsep = '\\';
    objext = ".obj";
    binext = ".exe";
    libext = ".lib";

    set_var("CC", "cl /nologo");
    set_var("LIB", "lib");
    set_var("LINK", "link");
    push_var("CPPFLAGS", "-Dinline=__inline");

    push_var("LDFLAGS", "/NOLOGO /DYNAMICBASE /NXCOMPAT /INCREMENTAL:NO /SUBSYSTEM:CONSOLE");

    if(mach.arch == ARM)
      push_var("LDFLAGS", "/MACHINE:ARM");
    else if(mach.bits == _64)
      push_var("LDFLAGS", "/MACHINE:X64");
    else if(mach.bits == _32)
      push_var("LDFLAGS", "/MACHINE:X86");

    /*    push_var("LDFLAGS",
                 "/DEBUG /DYNAMICBASE /INCREMENTAL /NXCOMPAT /TLBID:1");
    */
    push_var("LDFLAGS", "/MANIFEST /MANIFESTFILE:$@.embed.manifest");

    //  push_var("LDFLAGS", "/MANIFEST /manifest:embed2 /MANIFESTUAC:\"level=asInvoker uiAccess=false\"");

    stralloc_copys(&compile_command, "$(CC) $(CFLAGS) $(CPPFLAGS) $(DEFS) -c -Fo\"$@\" $<");
    stralloc_copys(&lib_command, "$(LIB) /OUT:$@ @<<\n\t\t$^\n<<");

    /*
     * Intel C++ compiler
     */
    if(str_start(type, "icl")) {
      set_var("CC", "icl /nologo");
      set_var("CXX", "icl /nologo");

      set_var("LINK", "xilink");
      set_var("LIB", "xilib");

      push_var("CFLAGS", "-Qip -Qunroll4 -nologo");

      //      stralloc_copys(&compile_command, "$(CC) $(CFLAGS) $(CPPFLAGS) $(DEFS) -c -Fo\"$@\" $<");
    }

    stralloc_copys(&link_command,
                   "$(LINK) -OUT:\"$@\" -INCREMENTAL -NOLOGO -MANIFEST "
                   "-MANIFESTFILE:Debug/genmk.exe.intermediate.manifest -DEBUG "
                   "-PDB:C:/Users/roman/Desktop/dirlist/genmk/Debug/genmk.pdb -SUBSYSTEM:CONSOLE "
                   "-DYNAMICBASE -NXCOMPAT -MACHINE:X86 -ERRORREPORT:PROMPT "
                   "@<<\n\t$(LDFLAGS)\n\t$^\n\t$(LDFLAGS) $(LIBS) $(EXTRA_LIBS)\n<<");

    /*
     * Borland C++ Builder
     */
  } else if(str_start(type, "bcc")) {

    pathsep = '\\';

    //    push_var("DEFS", "-DWIN32_LEAN_AND_MEAN");

    push_var("CFLAGS", "-q -tWC -tWM -O2");
    push_var("CPPFLAGS", "-Dinline=__inline");
    push_var("LDFLAGS", "-q");

    if(build_type == BUILD_TYPE_DEBUG) {
      push_var("CFLAGS", "-w -w-use");
    } else {
      push_var("CFLAGS", "-O1 -r");
    }

    /* Embracadero C++ */
    if(str_find(type, "55") == str_len(type)) {
      set_var("CC", "bcc32c");
      set_var("CXX", "bcc32x");

      /* C99 standard */
      push_var("CFLAGS", "-An");

      stralloc_copys(&link_command, "$(CC) $(LDFLAGS) -o $@ @&&|\n$^ $(LIBS) $(EXTRA_LIBS) $(STDC_LIBS)\n|");

      /* Borland C++ Builder 5.5 */
    } else {
      pathsep = '\\';

      set_var("CC", "bcc32");
      set_var("CXX", "bcc32");

      push_var("CFLAGS", "-ff -fp");

      if(build_type == BUILD_TYPE_DEBUG || build_type == BUILD_TYPE_RELWITHDEBINFO) {
        push_var("CFLAGS", "-v -y");
        push_var("LDFLAGS", "-v -y");
      }

      stralloc_copys(&compile_command, "$(CC) $(CFLAGS) $(CPPFLAGS) $(DEFS) -c -o$@ $<");
      stralloc_copys(&link_command, "$(CC) $(LDFLAGS) -e$@ @&&|\n$^ $(LIBS) $(EXTRA_LIBS) $(STDC_LIBS)\n|");
    }

    set_var("LINK", "ilink32");
    set_var("LIB", "tlib");

    push_lib("STDC_LIBS", "cw32");
    push_lib("STDC_LIBS", "import32");

    stralloc_copys(&lib_command, "$(LIB) @&&|\n\t/a /u \"$@\" $^\n|");

    /*
     * LCC compiler
     */
  } else if(str_start(type, "lcc")) {

    set_var("CC", "lcc");

    /*
     * Tiny CC compiler
     */
  } else if(str_start(type, "tcc")) {

    libext = ".a";
    format_linklib_fn = &format_linklib_switch;

    set_var("CC", "tcc");

    push_var("CFLAGS", "-g -Wall");
    push_var("CPPFLAGS", "-D__TCC__=1");

    stralloc_copys(&lib_command, "$(CC) -ar rcs $@ $^");
    stralloc_copys(&link_command, "$(CC) $(LDFLAGS) -o $@ $^ $(LIBS) $(EXTRA_LIBS)");
  } else {
    return 0;
  }

  with_lib("zlib");
  with_lib("bz2");
  with_lib("lzma");

  push_lib("EXTRA_LIBS", "advapi32");
  push_lib("EXTRA_LIBS", "ws2_32");
  push_lib("EXTRA_LIBS", "iphlpapi");
  push_lib("EXTRA_LIBS", "psapi");
  push_lib("EXTRA_LIBS", "shlwapi");

  return 1;
}
static stralloc tmp;

int
main(int argc, char* argv[]) {
  const char* type = "gnu";
  static int cmd_objs = 0, cmd_libs = 0, cmd_bins = 0;
  int c;
  int ret = 0, index = 0;
  const char *outfile = NULL, *dir = NULL;
  strlist workdir;
  strarray args;

  struct longopt opts[] = {{"help", 0, NULL, 'h'},
                           {"objext", 0, NULL, 'O'},
                           {"exeext", 0, NULL, 'B'},
                           {"libext", 0, NULL, 'L'},
                           {"create-libs", 0, &cmd_libs, 1},
                           {"create-objs", 0, &cmd_objs, 1},
                           {"create-bins", 0, &cmd_bins, 1},
                           {"builddir", 0, 0, 'd'},
                           {"type", 0, 0, 't'},
                           {"arch", 0, 0, 'a'},
                           {"release", 0, &build_type, BUILD_TYPE_RELEASE},
                           {"relwithdebinfo", 0, &build_type, BUILD_TYPE_RELWITHDEBINFO},
                           {"minsizerel", 0, &build_type, BUILD_TYPE_MINSIZEREL},
                           {"debug", 0, &build_type, BUILD_TYPE_DEBUG},
                           {0}};

  for(;;) {
    c = getopt_long(argc, argv, "ho:O:B:L:d:t:", opts, &index);
    if(c == -1) break;
    if(c == 0) continue;

    switch(c) {
      case 'h': usage(argv[0]); return 0;
      case 'o': outfile = optarg; break;
      case 'O': objext = optarg; break;
      case 'B': binext = optarg; break;
      case 'L': libext = optarg; break;
      case 'd': dir = optarg; break;
      case 't': type = optarg; break;
      case 'a': set_machine(optarg); break;
      default: usage(argv[0]); return 1;
    }
  }

  if(!cmd_bins && !cmd_libs && !cmd_objs) {
    cmd_bins = 1;
    cmd_objs = 1;
    cmd_libs = 1;
  }

  if(!format_linklib_fn) format_linklib_fn = &format_linklib_lib;

  if(!set_compiler(type)) {
    usage(argv[0]);
    return 2;
  }

  strlist_init(&workdir, pathsep);
  strlist_init(&outdir, pathsep);
  strlist_init(&builddir, pathsep);

  if(outfile) {
    int fd;
    if((fd = open_trunc(outfile)) != -1) buffer_1->fd = fd;

    path_dirname(outfile, &outdir.sa);

    if(stralloc_equals(&outdir.sa, "."))
      stralloc_zero(&outdir.sa);
    else
      stralloc_catc(&outdir.sa, pathsep);
  }

  path_getcwd(&workdir.sa);

  stralloc_nul(&outdir.sa);
  stralloc_nul(&workdir.sa);

  if(strlist_contains(&outdir, "build")) {
    path_relative(outdir.sa.s, workdir.sa.s, &builddir.sa);
  } else if(!strlist_contains(&workdir, "build")) {
    strlist_push(&builddir, dir ? dir : "build");
    strlist_push(&builddir, type);
  }

  strlist_push(&builddir, build_types[build_type]);

  stralloc_nul(&outdir.sa);
  stralloc_nul(&workdir.sa);
  stralloc_nul(&builddir.sa);

  debug_sa("outdir", &outdir.sa);
  debug_sa("workdir", &workdir.sa);
  debug_sa("builddir", &builddir.sa);

  if(outdir.sa.len) {
    path_relative(workdir.sa.s, outdir.sa.s, &tmp);

    if(tmp.len) {
      stralloc_catc(&tmp, pathsep);
      stralloc_copy(&srcdir, &tmp);
      debug_sa("srcdir: ", &srcdir);
    }
    stralloc_zero(&tmp);
  }

  path_relative(builddir.sa.s, outdir.sa.s, &tmp);
  /*
    if(tmp.len) {
      stralloc_catc(&tmp, pathsep);
      stralloc_copy(&builddir.sa, &tmp);
    }
    stralloc_free(&tmp);

    debug_sa("builddir", &builddir.sa);
  */
  strarray_init(&args);
  strarray_init(&srcs);

  while(optind < argc) {
#if WINDOWS_NATIVE && !MINGW
    if(str_rchrs(argv[optind], "*?", 2) < str_len(argv[optind]))
      strarray_glob(&args, argv[optind]);
    else
#endif
      strarray_push(&args, argv[optind]);
    ++optind;
  }

  /* No arguments given */
  if(strarray_size(&args) == 0) {

    buffer_putsflush(buffer_2, "ERROR: No arguments given\n\n");
    usage(argv[0]);
    return 1;

  } else {
    target* rule;
    char** arg;
    target* all = get_rule("all");

    if(strlist_count(&builddir)) {

      if((rule = get_rule_sa(&builddir.sa))) {
        rule->recipe = &mkdir_command;

        add_path_sa(&all->prereq, &builddir.sa);
      }
    }

    hmap_init(1024, &sourcedirs);

    strarray_foreach(&args, arg) {

      if(!path_exists(*arg)) {
        buffer_putm_internal(buffer_2, "ERROR: Doesn't exist: ", *arg, "\n", 0);
        buffer_flush(buffer_2);
        ret = 127;
        goto fail;
      }

      if(str_end(*arg, ".c"))
        add_source(*arg, &srcs);
      else
        get_sources(*arg, &srcs);
    }

    populate_sourcedirs(&srcs, sourcedirs);

    if(cmd_objs) compile_rules(rules, &srcs);
    if(cmd_libs) lib_rules(rules, sourcedirs);
    if(cmd_bins) link_rules(rules, &srcs);

    deps_for_libs(rules);

    clean_rule(rules);

  fail:
    output_all_vars(buffer_1, vars);
    output_all_rules(buffer_1, rules);

    //   hmap_dump(sourcedirs, buffer_1);

    hmap_destroy(&sourcedirs);
  }

  return ret;
}