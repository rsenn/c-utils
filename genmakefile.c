#include "lib/unix.h"
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
#include "lib/uint32.h"
#include "lib/windoze.h"

#include <ctype.h>

#if WINDOWS
#define MAX_CMD_LEN 1023

#define DEFAULT_OBJEXT ".obj"
#define DEFAULT_LIBEXT ".lib"
#define DEFAULT_LIBPFX ""
#define DEFAULT_EXEEXT ".exe"
#else
#define MAX_CMD_LEN 8191

#define DEFAULT_OBJEXT ".o"
#define DEFAULT_LIBEXT ".a"
#define DEFAULT_LIBPFX "lib"
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
static const char* libpfx = DEFAULT_LIBPFX;
static const char* binext = DEFAULT_EXEEXT;

static const char* make_begin_inline = NULL;
static const char* make_end_inline = NULL;

static strlist builddir, outdir;
static stralloc srcdir;
static char pathsep_make = DEFAULT_PATHSEP, pathsep_args = DEFAULT_PATHSEP;
static int build_type = BUILD_TYPE_DEBUG;

static HMAP_DB *sourcedirs, *rules, *vars;

static machine_type mach;

static linklib_fmt* format_linklib_fn;

void
set_command(stralloc* sa, const char* cmd, const char* args) {
  stralloc_copys(sa, cmd);
  if(args) {
    stralloc_catc(sa, ' ');
    if(make_begin_inline)
      stralloc_cats(sa, make_begin_inline);
    stralloc_cats(sa, args);
    if(make_end_inline)
      stralloc_cats(sa, make_end_inline);
  }
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

#ifdef DEBUG_OUTPUT

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
debug_sl(const char* name, const strlist* l) {
  size_t pos, n;
  const char* x;
  stralloc tmp;
  stralloc_init(&tmp);
  strlist_foreach(l, x, n) {
    if(tmp.len)
      stralloc_catc(&tmp, ' ');
    if((pos = byte_rchr(x, n, '/')) < n || (pos = byte_rchr(x, n, '\\')) < n)
      stralloc_catb(&tmp, x + pos + 1, n - pos - 1);
    else
      stralloc_catb(&tmp, x, n);
  }
  debug_sa(name, &tmp);
  stralloc_free(&tmp);
}

void
debug_int(const char* name, int i) {
  buffer_puts(buffer_2, name);
  buffer_puts(buffer_2, ": ");
  buffer_putlong(buffer_2, i);
  buffer_putnlflush(buffer_2);
}
#else
#define debug_sa(x, y)
#define debug_sl(x, y)
#define debug_s(x, y)
#define debug_int(x, y)
#endif

/**
 * @defgroup path functions
 * @{
 */

/**
 * @brief path_prefix_s Adds a prefix to the specified path
 * @param prefix        Prefix to add
 * @param path          The path string
 * @param out           Write output here
 */
void
path_prefix_s(const stralloc* prefix, const char* path, stralloc* out) {
  stralloc_zero(out);
  if(prefix->len) {
    stralloc_cat(out, prefix);

    if(!stralloc_endb(prefix, &pathsep_make, 1))
      stralloc_catc(out, pathsep_make);
  }
  stralloc_cats(out, path);
}

/**
 * @brief path_prefix_b Adds a prefix to the specified path
 * @param prefix        Prefix to add
 * @param x             The path buffer
 * @param n             Length of path buffer
 * @param out           Write output here
 */
void
path_prefix_b(const stralloc* prefix, const char* x, size_t n, stralloc* out) {
  stralloc_zero(out);
  if(prefix->len) {
    stralloc_cat(out, prefix);

    if(!stralloc_endb(prefix, &pathsep_make, 1))
      stralloc_catc(out, pathsep_make);
  }
  stralloc_catb(out, x, n);
}

/**
 * Change file extension and concatenate it to out.
 */
char*
path_extension(const char* in, stralloc* out, const char* ext) {
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
path_object(const char* in, stralloc* out) {
  stralloc_zero(out);

  if(builddir.sa.len) {
    stralloc_cat(out, &builddir.sa);

    if(!stralloc_endb(out, &pathsep_make, 1))
      stralloc_catc(out, pathsep_make);
  }

  return path_extension(str_basename(in), out, objext);
}

/**
 * @brief path_wildcard  Replaces the path basename (without extensions) with a wildcard
 * @param path           The path to replace
 * @param sa             Write output here
 * @return               Output string
 */
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
 * @}
 */

/**
 * Output library name (+".lib")
 */
void
format_linklib_lib(const char* libname, stralloc* out) {
  stralloc_cats(out, libpfx);
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

  if(stralloc_endb(out, "lib", 3))
    out->len -= 3;
}

/**
 * Checks if the given source file contains a main() function
 */
int
scan_main(const char* x, size_t n) {
  while(n) {
    size_t i = byte_finds(x, n, "main");
    if(i + 5 >= n)
      return 0;
    i += 4;
    x += i;
    n -= i;
    if(i > 4 && !isspace(*(x - 5)))
      continue;
    if((i = scan_whitenskip(x, n)) == n)
      break;
    x += i;
    n -= i;
    if(*x == '(')
      return 1;
  }
  return 0;
}

/**
 * @brief has_main  Checks for main() routine in source file
 * @param filename  Path to source file
 * @return          1 when yes, 0 when no, -1 on error
 */
int
has_main(const char* filename) {
  char* x;
  size_t n;
  if((x = (char*)mmap_read(filename, &n))) {
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
    if((i = scan_charsetnskip(x, " \t\r\n", n)) == n)
      break;
    x += i;
    n -= i;
    if(*x == '#') {
      x += 1;
      n -= 1;
      if((i = scan_charsetnskip(x, " \t\r", n) + 7) >= n)
        break;
      x += i;
      n -= i;
      if(!str_diffn(x - 7, "include", 7)) {
        char quote;
        if((i = scan_charsetnskip(x, " \t\r", n) + 1) >= n)
          break;
        x += i;
        n -= i;
        quote = *(x - 1);
        if((sys && quote == '<') || quote == '"') {
          char set[3];
          set[0] = (quote == '<' ? '>' : '"');
          set[1] = '\n';
          set[2] = '\0';
          if((i = scan_noncharsetnskip(x, set, n)) >= n)
            break;

          strlist_pushb_unique(includes, x, i);
          x += i + 1;
          n -= i + 1;
        }
      }
    }
    if((i = byte_chr(x, n, '\n')) >= n)
      break;
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
  stralloc prereq;
  stralloc_init(&prereq);
  stralloc_copy(&prereq, &rule->prereq.sa);
  stralloc_replace(&prereq, pathsep_args == '/' ? '\\' : '/', pathsep_args);

  for(i = 0; i < in->len; ++i) {
    const char* p = &in->s[i];

    if(i + 2 <= in->len && *p == '$' && str_chr("@^<", p[1]) < 3) {
      switch(p[1]) {
        case '@': {
          stralloc_cats(out, rule->name);
          break;
        }
        case '^': {

          stralloc_cat(out, &prereq);
          break;
        }
        case '<': {
          size_t n = stralloc_findb(&prereq, &rule->prereq.sep, 1);
          stralloc_catb(out, prereq.s, n);
          break;
        }
      }
      ++i;
    } else {
      if(!stralloc_append(out, p))
        break;
    }
  }
  stralloc_free(&prereq);
}

/**
 * Find or create rule
 */
target*
get_rule(const char* name) {
  target* ret = NULL;
  TUPLE* t;

  if(rules == NULL)
    hmap_init(1024, &rules);

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

    if(str_equal(name, needle))
      return t->vals.val_custom;

    if(str_equal(str_basename(name), str_basename(needle)))
      return t->vals.val_custom;

    if(t->next == rules->list_tuple)
      break;
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
    if(list->sa.s[i] == '/' || list->sa.s[i] == '\\')
      list->sa.s[i] = pathsep_make;
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
 * @defgroup source functions
 * @{
 */

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
 * Adds a source file to the given list.
 */
void
add_source(const char* filename, strarray* sources) {
  if(str_end(filename, ".c")) {
    stralloc sa;
    stralloc_init(&sa);
    stralloc_copys(&sa, filename);
    //    stralloc_replace(&sa, pathsep_make == '/' ? '\\' : '/', pathsep_make);

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
 * @}
 */

/**
 * @defgroup var functions
 * @{
 */

/**
 * Find or create variable
 */
strlist*
get_var(const char* name) {
  TUPLE* t;

  if(vars == NULL)
    hmap_init(1024, &vars);

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

  if(var->sa.len)
    stralloc_catc(&var->sa, var->sep);

  format_linklib_fn(lib, &var->sa);
}

void
with_lib(const char* lib) {
  stralloc def;
  stralloc_init(&def);
  stralloc_copys(&def, "-DHAVE_");

  if(str_find(lib, "lib") == str_len(lib))
    stralloc_cats(&def, "LIB");

  stralloc_cats(&def, lib);
  stralloc_cats(&def, "=1");
  byte_upper(def.s, def.len);

  push_var_sa("DEFS", &def);

  push_lib("LIBS", lib);
}

/**
 * @}
 */

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
 * Gets directory name from a file path (allocated).
 */
char*
dirname_alloc(const char* p) {
  size_t len = str_len(p);
  size_t pos = str_rchrs(p, "\\/", 2);

  if(pos < len)
    return str_ndup(p, pos);

  return str_dup(".");
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

      stralloc_replace(&l.sa, pathsep_make == '\\' ? '/' : '\\', pathsep_make);

      strlist_foreach_s(&l, s) {
        dir.len = dlen;
        stralloc_catc(&dir, pathsep_make);
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

    stralloc_copys(&sa, libpfx);
    stralloc_catb(&sa, s, n);
    // debug_sa("include", &sa);

    if(stralloc_endb(&sa, ".h", 2))
      sa.len -= 2;
    if(stralloc_endb(&sa, "_internal", 9))
      sa.len -= 9;

    stralloc_cats(&sa, libext);

    //    debug_sa("includes_to_libs", &sa);

    if((rule = find_rule_sa(&sa))) {

      // debug_s("lib", rule->name);

      strlist_push(libs, rule->name);
    }
  }
  stralloc_free(&sa);
}

/**
 * @brief target_ptrs  Given a list of target names, outputs an array of pointers to those targets.
 * @param targets      Target names
 * @param out          Output array
 */
void
target_ptrs(const strlist* targets, array* out) {
  const char* x;
  size_t n;

  strlist_foreach(targets, x, n) {
    target* rule;

    if((rule = find_rule_b(x, n))) {
      if(!array_find(out, sizeof(target*), &rule))
        array_catb(out, &rule, sizeof(target*));
    } else {
      buffer_puts(buffer_2, "ERROR: rule '");
      buffer_put(buffer_2, x, n);
      buffer_puts(buffer_2, "' not found");
      buffer_putnlflush(buffer_2);
    }
  }
}

/**
 * @brief target_dep_list_recursive   Lists all dependencies of a target
 * @param l                           Output target names
 * @param t                           Target
 */
void
target_dep_list_recursive(strlist* l, target* t, int depth, strlist* hier) {
  target** ptr;

  array_foreach_t(&t->deps, ptr) {
    const char* name = (*ptr)->name;

    if(!strlist_contains(hier, name)) {
      strlist_push(hier, name);
      target_dep_list_recursive(l, *ptr, depth + 1, hier);
      strlist_pop(hier);

      if(depth >= 0) {
        if(!strlist_contains(l, name))
          strlist_unshift(l, name);
      }
    }
  }
}

void
target_dep_list(strlist* l, target* t) {
  strlist hier;
  strlist_init(&hier, '\0');
  strlist_push(&hier, t->name);

  strlist_zero(l);

  target_dep_list_recursive(l, t, 0, &hier);
  strlist_free(&hier);
}

/**
 * @brief indirect_dep_list  List all indirect deps of a target
 * @param l                  Output target names
 * @param t                  Target
 */
void
target_deps_indirect(strlist* l, target* t) {
  target** ptr;
  strlist hier;
  strlist_init(&hier, '\0');
  strlist_push(&hier, t->name);

  strlist_push_unique(l, t->name);

  array_foreach_t(&t->deps, ptr) {
    if(*ptr)
      target_dep_list_recursive(l, *ptr, 0, &hier);
  }

  strlist_removes(l, t->name);
  strlist_free(&hier);
}

void
deps_indirect(strlist* l, const strlist* names) {
  size_t n;
  const char* x;
  target* t;
  strlist hier;
  strlist_init(&hier, '\0');

  strlist_foreach(names, x, n) {
    if((t = find_rule_b(x, n))) {
      strlist_pushb(&hier, x, n);
      target_dep_list_recursive(l, t, -1, &hier);
      strlist_zero(&hier);
    }
  }
  strlist_free(&hier);
}

void
deps_direct(strlist* l, const target* t) {
  target** ptr;
  array_foreach_t(&t->deps, ptr) {
    if(*ptr) {
      strlist_push(l, (*ptr)->name);
    }
  }
}

void
print_target_deps_r(buffer* b, target* t, strlist* deplist, strlist* hierlist, int depth) {
  target** ptr;
  size_t l = hierlist->sa.len;

  /*if(strlist_contains(&hierlist, t->name))
    return;
  */
  strlist_push(hierlist, t->name);
  stralloc_nul(&hierlist->sa);

  array_foreach_t(&t->deps, ptr) {
    const char* name = (*ptr)->name;

    if(strlist_contains(hierlist, name))
      continue;

    // strlist_push_unique(deplist, name);

    {
      buffer_puts(b, "# ");
      //   buffer_putsa(b, &hierlist->sa);
      buffer_putnspace(b, depth * 2);

      buffer_puts(b, str_basename(t->name));
      buffer_puts(b, " -> ");
      buffer_puts(b, str_basename(name));
      buffer_putnlflush(b);

      if(strlist_push_unique(deplist, name))
        print_target_deps_r(b, (*ptr), deplist, hierlist, depth + 1);
    }
  }

  hierlist->sa.len = l;
  //   strlist_trunc(hierlist, depth);
}

/**
 * @brief print_target_deps  Prints dependency tree for a target
 * @param b                  Output buffer
 * @param t                  Target
 */
void
print_target_deps(buffer* b, target* t) {
  const char* s;
  size_t n, nb;
  strlist deplist, hierlist;
  strlist_init(&deplist, ' ');
  strlist_init(&hierlist, ',');

  strlist_push(&deplist, t->name);

  buffer_putm_internal(b, "# Dependencies for '", t->name, "':", 0);
  buffer_putnlflush(b);

  print_target_deps_r(b, t, &deplist, &hierlist, 0);

  strlist_free(&deplist);
  strlist_free(&hierlist);
}

/**
 * @brief remove_indirect_deps_recursive   Removes all indirect dependencies
 * @param top                              Toplevel dependencies
 * @param a                                Dependency layer array
 * @param depth                            Recursion depth
 */
void
remove_indirect_deps_recursive(array* top, array* a, int depth) {
  target **p, **found;

  array_foreach_t(a, p) {
    target* t = *p;

    if(t == NULL)
      continue;

    if(depth > 0) {
      if((found = array_find(top, sizeof(target*), &t))) {
        *found = NULL;
      }
    }
    if(a != &t->deps) {
      if(depth < 100 && array_length(&t->deps, sizeof(target*)) > 0)
        remove_indirect_deps_recursive(top, &t->deps, depth + 1);
    }
  }
}

int
remove_indirect_deps(array* deps) {
  size_t w, r, n;
  target** a;

  remove_indirect_deps_recursive(deps, deps, 0);

  n = array_length(deps, sizeof(target*));
  a = array_start(deps);

  for(w = 0, r = 0; r < n; ++r) {
    if(a[r])
      a[w++] = a[r];
  }
  array_truncate(deps, sizeof(target*), w);
  return r - w;
}

/**
 * Output rule to buffer
 */
void
output_rule(buffer* b, target* rule) {
  int num_deps = strlist_count(&rule->prereq);

/*  if(array_length(&rule->deps, sizeof(target*))) {
    print_target_deps(b, rule);
  }*/

  if(num_deps == 0 && str_diffn(rule->name, builddir.sa.s, builddir.sa.len)) {
    buffer_putm_internal(b, ".PHONY: ", rule->name, "\n", 0);
  }

  buffer_puts(b, rule->name);
  buffer_putc(b, ':');

  if(num_deps) {
    stralloc prereq;
    stralloc_init(&prereq);
    stralloc_copy(&prereq, &rule->prereq.sa);
    stralloc_replace(&prereq, pathsep_make == '/' ? '\\' : '/', pathsep_make);

    buffer_putspace(b);
    buffer_putsa(b, &prereq);

    stralloc_free(&prereq);
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
 * @defgroup source dir functions
 * @{
 */

/**
 * @brief get_sourcedir  Searches for a source directory
 * @param path           Path string
 * @return               Pointer to sourcedir structure or NULL
 */
sourcedir*
get_sourcedir(const char* path) {
  return hmap_get(sourcedirs, path, str_len(path) + 1);
}

/**
 * @brief get_sourcedir_sa Searches for a source directory
 * @param path             Path stralloc
 * @return                 Pointer to sourcedir structure or NULL
 */
sourcedir*
get_sourcedir_sa(stralloc* path) {
  stralloc_nul(path);
  return hmap_get(sourcedirs, path->s, path->len + 1);
}

/**
 * @brief get_sourcedir_b  Searches for a source directory
 * @param x                Path buffer
 * @param n                Length of path
 * @return               Pointer to sourcedir structure or NULL
 */
sourcedir*
get_sourcedir_b(const char* x, size_t n) {
  sourcedir* ret;
  stralloc p;
  stralloc_init(&p);
  stralloc_copyb(&p, x, n);
  ret = get_sourcedir_sa(&p);
  stralloc_free(&p);
  return ret;
}
/**
 * Generate lib rule for source dir
 */
target*
lib_rule_for_sourcedir(HMAP_DB* rules, sourcedir* srcdir, const char* name) {
  target* rule;
  stralloc sa;
  stralloc_init(&sa);

  path_prefix_s(&builddir.sa, name, &sa);

  stralloc_cats(&sa, libext);

  //  debug_sa("lib_rule_for_sourcedir", &sa);

  if((rule = get_rule_sa(&sa))) {
    sourcefile* pfile;
    strlist_init(&rule->prereq, ' ');

    slist_foreach(srcdir->sources, pfile) {
      path_object(pfile->name, &sa);

      add_path_sa(&rule->prereq, &sa);
    }

    rule->recipe = &lib_command;
  }

  stralloc_free(&sa);
  return rule;
}

/**
 * @}ü
 */

void
deps_for_libs(HMAP_DB* rules) {
  TUPLE* t;
  strlist deps, indir;
  stralloc sa;
  strlist_init(&deps, '\0');
  strlist_init(&indir, ' ');
  stralloc_init(&sa);

  hmap_foreach(sourcedirs, t) {
    sourcedir* srcdir = hmap_data(t);
    target* lib;
    size_t n;
    const char* s;

    path_prefix_s(&builddir.sa, str_basename(t->key), &sa);
    stralloc_cats(&sa, libext);

    if((lib = find_rule_sa(&sa))) {
      strlist libs;
      strlist_init(&libs, ' ');

      //      debug_sl("includes", &srcdir->includes);

      includes_to_libs(&srcdir->includes, &libs);

      debug_s("library", lib->name);

      strlist_removes(&libs, lib->name);
      debug_sl("deps", &libs);

      strlist_zero(&indir);
      deps_indirect(&indir, &libs);

      debug_sl("indir", &indir);
      strlist_sub(&indir, &libs);

      strlist_sub(&libs, &indir);

      debug_sl("direct", &libs);
#if 0 // def DEBUG_OUTPUT
      //print_target_deps(buffer_2, lib);
      buffer_putm_internal(buffer_2, "Deps for library '", lib->name, "': ", 0);
      buffer_putsa(buffer_2, &libs.sa);
      buffer_putnlflush(buffer_2);
#endif

      target_ptrs(&libs, &lib->deps);

      // print_target_deps(buffer_2, lib);

      strlist_free(&libs);
    }
  }
  stralloc_free(&sa);
}

int
target_add_dep(target* t, target* other) {

  target** ptr;

  if((ptr = array_find(&t->deps, sizeof(target*), &other)) == NULL) {
    array_catb(&t->deps, &other, sizeof(other));

    array_foreach_t(&other->deps, ptr) { target_add_dep(t, *ptr); }
    return 1;
  }
  return 0;
}

void
target_add_deps(target* t, const strlist* deps) {
  const char* x;
  size_t n;

  strlist_foreach(deps, x, n) {
    target *other, **ptr;

    if(str_len(t->name) == n && !str_diffn(t->name, x, n))
      continue;

    if((other = find_rule_b(x, n))) {
      target_add_dep(t, other);
    }
  }
}

/**
 * Generate compile rules for every source file given
 */
void
gen_compile_rules(HMAP_DB* rules, strarray* sources) {
  char** srcfile;
  stralloc obj;
  stralloc_init(&obj);

  strarray_foreach(sources, srcfile) {
    target* rule;

    path_object(*srcfile, &obj);

    if((rule = get_rule_sa(&obj))) {
      add_srcpath(&rule->prereq, *srcfile);

      rule->recipe = &compile_command;
    }
  }

  stralloc_free(&obj);
}

/**
 * Generate compile rules for every library given
 */
void
gen_lib_rules(HMAP_DB* rules, HMAP_DB* srcdirs) {
  target* rule;
  TUPLE* t;
  stralloc inc;
  stralloc_init(&inc);

  hmap_foreach(srcdirs, t) {
    sourcedir* srcdir = hmap_data(t);
    const char *s, *base = str_basename(t->key);
    size_t n;

    if(str_equal(base, "lib") || base[0] == '.' || base[0] == '\0')
      continue;

    rule = lib_rule_for_sourcedir(rules, srcdir, base);
  }
  stralloc_free(&inc);
}

/**
 * Generate compile rules for every source file with a main()
 */
void
gen_link_rules(HMAP_DB* rules, strarray* sources) {
  target* all;

  const char* x;
  char** srcfile;
  strlist incs, libs, deps, indir;
  stralloc obj, bin;
  strlist_init(&incs, ' ');
  strlist_init(&libs, ' ');
  strlist_init(&deps, ' ');
  strlist_init(&indir, ' ');
  stralloc_init(&obj);
  stralloc_init(&bin);
  all = get_rule("all");

  strarray_foreach(sources, srcfile) {
    target *compile, *link;

    strlist_zero(&incs);
    strlist_zero(&libs);
    strlist_zero(&deps);
    strlist_zero(&indir);

    if(has_main(*srcfile) == 1) {

      path_object(*srcfile, &obj);

      if((compile = find_rule_sa(&obj))) {

        get_includes(*srcfile, &incs, 0);

        /*        stralloc_nul(&incs);
                buffer_putm_internal(buffer_2, "rule '", compile->name, "' includes: ", incs.sa.s, 0);
                buffer_putnlflush(buffer_2);
        */
        includes_to_libs(&incs, &libs);
      }

      stralloc_zero(&bin);
      path_extension(obj.s, &bin, binext);

      add_path_sa(&all->prereq, &bin);

      if((link = get_rule_sa(&bin))) {
        int nremoved;
        add_path_sa(&link->prereq, &obj);

        //  get_rules_by_cmd(&lib_command, &link->prereq);

        link->recipe = &link_command;

        debug_s("program", link->name);
        debug_sa("program libs", &libs);

        /*        deps_indirect(&indir, &libs);

                strlist_sub(&libs, &indir);

                target_add_deps(link, &libs);

                strlist_zero(&deps);
                target_dep_list(&deps, link);


                debug_sa("final deps", &deps);
        */

        /*
        strlist_zero(&deps);
        strlist_cat(&deps, &libs);

        deps_direct(&deps, link);


        strlist_sub(&deps, &indir);
             debug_sa("direct deps", &deps);

        array_trunc(&link->deps);


        */
        target_ptrs(&libs, &link->deps);

        strlist_zero(&deps);
        target_dep_list(&deps, link);

        strlist_cat(&link->prereq, &deps);

#if 0 // def DEBUG_OUTPUT
        /*print_target_deps(buffer_2, link);
        buffer_putm_internal(buffer_2, "Deps for executable '", link->name, "': ", 0);
        buffer_putsa(buffer_2, &deps.sa);
        buffer_putnlflush(buffer_2);*/
#endif
      }
    }
  }

  strlist_free(&incs);
  strlist_free(&libs);
  strlist_free(&deps);
  strlist_free(&indir);
  stralloc_free(&bin);
  stralloc_free(&obj);
}

void
gen_clean_rule(HMAP_DB* rules) {
  target* rule;

  /* Generate "clean" rule */
  if((rule = get_rule("clean"))) {
    TUPLE* t;
    char* arg;
    uint32 cmdoffs, lineoffs = 0;
    stralloc fn;
    strlist delete_args;
    stralloc_init(&fn);
    strlist_init(&delete_args, '\0');

    if(delete_command.len == 0)
      stralloc_copys(&delete_command, "DEL /F /Q");

    cmdoffs = delete_command.len;

    hmap_foreach(rules, t) {

      /* Ignore the builddir rule */
      if(stralloc_equals(&builddir.sa, t->key))
        continue;

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
        stralloc_readyplus(&delete_command, cmdoffs + 3);
        stralloc_cats(&delete_command, "\n\t");
        stralloc_catb(&delete_command, delete_command.s, cmdoffs);

        lineoffs = delete_command.len;
      }

      stralloc_catc(&delete_command, ' ');
      stralloc_cats(&delete_command, arg);

      if(arg[str_chr(arg, '*')])
        lineoffs = -MAX_CMD_LEN;
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
                       "  -t, --compiler-type TYPE   compiler type, one of:\n"
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
                       "  -m, --make-type TYPE      make program type, one of:\n"
                       "\n"
                       "     nmake       Microsoft NMake\n"
                       "     borland     Borland Make\n"
                       "     gmake       GNU Make\n"
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
 * Set make program type
 */
int
set_make_type(const char* make, const char* compiler) {

  stralloc_copys(&mkdir_command, "IF NOT EXIST \"$@\" MKDIR \"$@\"");

  if(str_start(make, "bmake") || str_start(make, "borland")) {

    /* Borland C++ Builder Make */
    pathsep_make = '\\';
    make_begin_inline = "@&&|\n\t";
    make_end_inline = "\n|";

  } else if(str_start(make, "nmake")) {

    /* Microsoft NMake */
    pathsep_make = '\\';
    make_begin_inline = "@<<\n\t";
    make_end_inline = "\n<<";

  } else if(str_start(make, "gmake") || str_start(make, "gnu")) {

    pathsep_make = '/';
    stralloc_copys(&mkdir_command, "test -d \"$@\" || mkdir -p \"$@\"");
    stralloc_copys(&delete_command, "rm -f");

  } else {
    return 0;
  }

  pathsep_args = pathsep_make;

  return 1;
}

/**
 * Set the compiler type
 */
int
set_compiler_type(const char* compiler) {

  push_var("CC", "cc");
  push_var("CXX", "c++");

  stralloc_copys(&compile_command, "$(CC) $(CFLAGS) $(CPPFLAGS) $(DEFS) -c -o $@ $<");
  stralloc_copys(&link_command, "$(LINK) -subsystem console -o $@ $(LDFLAGS) $^ $(LIBS) $(EXTRA_LIBS)");
  stralloc_copys(&lib_command, "$(LIB) /out:$@ $^");

  push_var("CFLAGS", "-O2");
  push_var("DEFS", "-DHAVE_ERRNO_H=1");

  if(str_start(compiler, "gnu") || str_start(compiler, "gcc") || str_start(compiler, "clang") ||
     str_start(compiler, "llvm")) {

    libext = ".a";
    objext = ".o";

    /*
     * GNU GCC compatible compilers
     */

    if(str_start(compiler, "gnu") || str_start(compiler, "gcc")) {
      set_var("CC", "gcc");
      set_var("CXX", "g++");

      set_var("AR", "ar");
    } else if(str_start(compiler, "clang") || str_start(compiler, "llvm")) {
      pathsep_args = '/';

      set_var("CC", "clang");
      set_var("CXX", "clang++");

      set_var("AR", "llvm-ar");
    }

    set_command(&lib_command, "$(AR) rcs $@", "$^");
    set_command(&link_command, "$(CC) $(CFLAGS) $(LDFLAGS) -o $@", "$^ $(LIBS) $(EXTRA_LIBS)");

    format_linklib_fn = &format_linklib_switch;

    /*
     * Visual C++ compiler
     */
  } else if(str_start(compiler, "msvc") || str_start(compiler, "icl")) {

    objext = ".obj";
    binext = ".exe";
    libext = ".lib";

    set_var("CC", "cl /nologo");
    set_var("LIB", "lib");
    set_var("LINK", "link");
    push_var("CPPFLAGS", "-Dinline=__inline");


    /*    push_var("LDFLAGS",
                 "/DEBUG /DYNAMICBASE /INCREMENTAL /NXCOMPAT /TLBID:1");
    */
    push_var("LDFLAGS", "/SUBSYSTEM:CONSOLE /TLBID:1 /DYNAMICBASE /NXCOMPAT");

    //  push_var("LDFLAGS", "/MANIFEST /manifest:embed2 /MANIFESTUAC:\"level=asInvoker uiAccess=false\"");

    stralloc_copys(&compile_command, "$(CC) $(CFLAGS) $(CPPFLAGS) $(DEFS) -c -Fo\"$@\" $<");
  set_command(&lib_command, "$(LIB) /out:$@", "$^");
    //    stralloc_copys(&lib_command, "$(LIB) /OUT:$@ @<<\n\t\t$^\n<<");

    /*
     * Intel C++ compiler
     */
    if(str_start(compiler, "icl")) {
      set_var("CC", "icl /nologo");
      set_var("CXX", "icl /nologo");

      set_var("LINK", "xilink");
      set_var("LIB", "xilib");

      push_var("CFLAGS", "-Qip -Qunroll4 -nologo");
      
      if(mach.bits == _64)
        push_var("LDFLAGS", "/LIBPATH:\"%ROOT%\\compiler\\lib\\intel64\"");
      else
        push_var("LDFLAGS", "/LIBPATH:\"%ROOT%\\compiler\\lib\"");

      //      stralloc_copys(&compile_command, "$(CC) $(CFLAGS) $(CPPFLAGS) $(DEFS) -c -Fo\"$@\" $<");
    }

    push_var("LDFLAGS", "/LIBPATH:\"%UniversalCRTSdkDir%lib\\%WindowsSDKLibVersion%ucrt\\$(MACHINE)\"");
    push_var("LDFLAGS", "/LIBPATH:\"%WindowsSdkDir%lib\\%WindowsSDKLibVersion%um\\$(MACHINE)\"");
    push_var("LDFLAGS", "/LIBPATH:\"%VCToolsInstallDir%lib\\$(MACHINE)\"");

    push_var("LDFLAGS", "/INCREMENTAL /MANIFEST /manifest:embed /MANIFESTUAC:\"level='asInvoker' uiAccess='false'\" /DEBUG");

    if(mach.arch == ARM) {
      push_var("LDFLAGS", "/MACHINE:ARM");
      set_var("MACHINE", mach.bits == _64 ? "arm64" : "arm");
    } else if(mach.bits == _64) {
      push_var("LDFLAGS", "/MACHINE:X64");
      set_var("MACHINE", "x64");
    } else if(mach.bits == _32) {
      push_var("LDFLAGS", "/MACHINE:X86");
      set_var("MACHINE", "x86");
    }
    
    set_command(&link_command,
                "$(LINK) /OUT:\"$@\" $(LDFLAGS) /PDB:\"$@.pdb\"", "$^ $(LIBS) $(EXTRA_LIBS)");

    /*
     * Borland C++ Builder
     */
  } else if(str_start(compiler, "bcc")) {

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
    if(str_find(compiler, "55") == str_len(compiler) && str_find(compiler, "60") == str_len(compiler)) {
      set_var("CC", "bcc32c");
      set_var("CXX", "bcc32x");

      /* C99 standard */
      push_var("CFLAGS", "-An");

      set_command(&link_command, "$(CC) $(LDFLAGS) -o $@ ", "$^ $(LIBS) $(EXTRA_LIBS) $(STDC_LIBS)");

      /* Borland C++ Builder 5.5 */
    } else {
      set_var("CC", "bcc32");
      set_var("CXX", "bcc32");

      push_var("CFLAGS", "-ff -fp");

      if(build_type == BUILD_TYPE_DEBUG || build_type == BUILD_TYPE_RELWITHDEBINFO) {
        push_var("CFLAGS", "-v -y");
        push_var("LDFLAGS", "-v -y");
      }

      stralloc_copys(&compile_command, "$(CC) $(CFLAGS) $(CPPFLAGS) $(DEFS) -c -o\"$@\" $<");
      set_command(&link_command, "$(CC) $(LDFLAGS) -e\"$@\"", "$^ $(LIBS) $(EXTRA_LIBS) $(STDC_LIBS)");
    }

    set_var("LINK", "ilink32");
    set_var("LIB", "tlib");

    push_lib("STDC_LIBS", "cw32");
    push_lib("STDC_LIBS", "import32");

    set_command(&lib_command, "$(LIB)", "/a /u \"$@\" $^");

    /*
     * LCC compiler
     */
  } else if(str_start(compiler, "lcc")) {

    if(mach.bits == _64) {
      set_var("CC", "lcc64");
      set_var("LINK", "lcclnk64");
      set_var("LIB", "lcclib64");
    } else {
      set_var("CC", "lcc");
      set_var("LINK", "lcclnk");
      set_var("LIB", "lcclib");
    }
    
    /*
     * Tiny CC compiler
     */
  } else if(str_start(compiler, "tcc")) {

    libext = ".a";
    objext = ".o";
    format_linklib_fn = &format_linklib_switch;

    set_var("CC", "tcc");

    push_var("CFLAGS", "-g -Wall");
    push_var("CPPFLAGS", "-D__TCC__=1");

    set_command(&lib_command, "$(CC) -ar rcs $@", "$^");
    set_command(&link_command, "$(CC) $(LDFLAGS) -o $@", "$^ $(LIBS) $(EXTRA_LIBS)");
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
  const char *compiler = NULL, *make = NULL;
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
                           {"compiler-type", 0, 0, 't'},
                           {"make-type", 0, 0, 'm'},
                           {"arch", 0, 0, 'a'},
                           {"release", 0, &build_type, BUILD_TYPE_RELEASE},
                           {"relwithdebinfo", 0, &build_type, BUILD_TYPE_RELWITHDEBINFO},
                           {"minsizerel", 0, &build_type, BUILD_TYPE_MINSIZEREL},
                           {"debug", 0, &build_type, BUILD_TYPE_DEBUG},
                           {0}};

  for(;;) {
    c = getopt_long(argc, argv, "ho:O:B:L:d:t:m:", opts, &index);
    if(c == -1)
      break;
    if(c == 0)
      continue;

    switch(c) {
      case 'h': usage(argv[0]); return 0;
      case 'o': outfile = optarg; break;
      case 'O': objext = optarg; break;
      case 'B': binext = optarg; break;
      case 'L': libext = optarg; break;
      case 'd': dir = optarg; break;
      case 't': compiler = optarg; break;
      case 'm': make = optarg; break;
      case 'a': set_machine(optarg); break;
      default: usage(argv[0]); return 1;
    }
  }

  if(!cmd_bins && !cmd_libs && !cmd_objs) {
    cmd_bins = 1;
    cmd_objs = 1;
    cmd_libs = 1;
  }

  if(!format_linklib_fn)
    format_linklib_fn = &format_linklib_lib;

  if(make == NULL && compiler) {
    if(str_start(compiler, "b"))
      make = "borland";
    else if(str_start(compiler, "msvc"))
      make = "nmake";
    else if(str_start(compiler, "g"))
      make = "gmake";
  }

  if(make == NULL)
    make = "gmake";

  if(compiler == NULL)
    compiler = "gcc";
  else if(mach.bits == 0)
    set_machine(compiler);

  if(!set_make_type(make, compiler) || !set_compiler_type(compiler)) {
    usage(argv[0]);
    return 2;
  }

  strlist_init(&workdir, pathsep_make);
  strlist_init(&outdir, pathsep_make);
  strlist_init(&builddir, pathsep_make);

  if(outfile) {
    int fd;
    if((fd = open_trunc(outfile)) != -1)
      buffer_1->fd = fd;

    path_dirname(outfile, &outdir.sa);

    if(stralloc_equals(&outdir.sa, "."))
      stralloc_zero(&outdir.sa);
    else
      stralloc_catc(&outdir.sa, pathsep_make);
  }

  path_getcwd(&workdir.sa);

  stralloc_nul(&outdir.sa);
  stralloc_nul(&workdir.sa);

  if(strlist_contains(&outdir, "build")) {
    path_relative(outdir.sa.s, workdir.sa.s, &builddir.sa);
  } else if(!strlist_contains(&workdir, "build")) {
    strlist_push(&builddir, dir ? dir : "build");
    strlist_push(&builddir, compiler);
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
      stralloc_catc(&tmp, pathsep_make);
      stralloc_copy(&srcdir, &tmp);
      debug_sa("srcdir: ", &srcdir);
    }
    stralloc_zero(&tmp);
  }

  path_relative(builddir.sa.s, outdir.sa.s, &tmp);
  /*
    if(tmp.len) {
      stralloc_catc(&tmp, pathsep_make);
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

    if(cmd_objs)
      gen_compile_rules(rules, &srcs);

    if(cmd_libs) {
      gen_lib_rules(rules, sourcedirs);

      deps_for_libs(rules);
    }

    if(cmd_bins)
      gen_link_rules(rules, &srcs);

    gen_clean_rule(rules);

    {
      TUPLE* t;
      hmap_foreach(rules, t) {
        target* tgt = hmap_data(t);
        // print_target_deps(buffer_2, tgt);
      }
    }

  fail:
    output_all_vars(buffer_1, vars);
    output_all_rules(buffer_1, rules);

    //   hmap_dump(sourcedirs, buffer_1);

    hmap_destroy(&sourcedirs);
  }

  return ret;
}
