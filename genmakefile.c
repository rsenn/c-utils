#include "lib/windoze.h"
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
#include "lib/errmsg.h"
#include "lib/array.h"
#include "lib/byte.h"
#include "lib/fmt.h"
#include "lib/dir.h"
#include "lib/range.h"
#include "lib/case.h"

#include <stdlib.h>
#include <ctype.h>
#include <sys/stat.h>
#include <sys/types.h>

extern buffer* optbuf;

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
  enum { WINDOWS, MAC, LINUX } os;
  enum { WIN, UNIX } type;
} system_type;

typedef struct {
  struct slink link;
  const char* name;
  int has_main;
} sourcefile;

typedef struct {
  int n_sources;
  slink* sources;
  strlist includes;
  array rules;
} sourcedir;

typedef struct {
  const char* name;
  strlist output;
  strlist prereq;
  stralloc recipe;
  array deps;
  array objs;
  uint32 serial;
} target;

enum {
  BUILD_TYPE_RELEASE = 0,
  BUILD_TYPE_RELWITHDEBINFO,
  BUILD_TYPE_MINSIZEREL,
  BUILD_TYPE_DEBUG,
};

void debug_int(const char* name, int i);
void debug_sa(const char* name, stralloc* sa);
void debug_sl(const char* name, const strlist* l);
void debug_s(const char* name, const char* s);

const char* const build_types[] = {"Release", "RelWithDebInfo", "MinSizeRel", "Debug"};

typedef void(linklib_fmt)(const char*, stralloc*);

static strarray srcs;
static stralloc compile_command, lib_command, link_command, mkdir_command, delete_command;
static const char* objext = DEFAULT_OBJEXT;
static const char* libext = DEFAULT_LIBEXT;
static const char* libpfx = DEFAULT_LIBPFX;
static const char* binext = DEFAULT_EXEEXT;

static const char *make_begin_inline, *make_sep_inline, *make_end_inline;
static const char* comment = "#";
static const char* cross_compile = "";

static strlist builddir, workdir;
static stralloc srcdir;
static char pathsep_make = DEFAULT_PATHSEP, pathsep_args = DEFAULT_PATHSEP;
static int build_type = -1;
static strlist build_as_lib;

static HMAP_DB *sourcedirs, *rules, *vars;

static const char *toolchain, *compiler, *make;
static const char* newline = "\n";
static machine_type mach;
static system_type sys;
static int batch, shell, ninja;
static int batchmode;

static linklib_fmt* format_linklib_fn;

static int inst_bins, inst_libs;
static int cygming;

#ifndef _WIN32
#define _mkdir mkdir
#endif

int
mkdir_sa(const stralloc* dir, int mode) {
  stralloc sa;
  stralloc_init(&sa);
  stralloc_copy(&sa, dir);
  if(stralloc_endb(&sa, &pathsep_make, 1))
    sa.len -= 1;
  stralloc_nul(&sa);
  return _mkdir(sa.s, mode);
}

int
mkdir_components(strlist* dir, int mode) {
  int ret = 0;
  size_t i, n = strlist_count(dir);

  for(i = 1; i <= n; ++i) {
    strlist r = strlist_range(dir, 0, i);

#ifdef DEBUG
    debug_sa("mkdir_components", &r.sa);
#endif
    if(mkdir_sa(&r.sa, mode) == -1)
      ret |= -1;
  }
  return ret;
}

void
stralloc_weak(stralloc* out, const stralloc* from) {
  if(out->a)
    stralloc_free(out);
  out->a = 0;
  out->s = from->s;
  out->len = from->len;
}

/**
 * @brief put_newline
 * @param b
 * @param flush
 */
void
put_newline(buffer* b, int flush) {
  buffer_puts(b, newline);
  if(flush)
    buffer_flush(b);
}

/**
 * @brief set_command
 * @param sa
 * @param cmd
 * @param args
 */
void
set_command(stralloc* sa, const char* cmd, const char* args) {
  stralloc_copys(sa, cmd);
  if(args) {
    stralloc_catc(sa, ' ');

    if(!(ninja || batch) && (make_begin_inline && make_end_inline)) {
      stralloc_cats(sa, make_begin_inline);
      if(!str_start(make, "nmake"))
        stralloc_subst(sa, args, str_len(args), "$^", "$|");
      else
        stralloc_copys(sa, args);

      stralloc_cats(sa, make_end_inline);
    } else {
      stralloc_cats(sa, args);
    }
  }
  if(str_start(make, "nmake"))
    stralloc_replaces(sa, "$^", "$**");
}

/**
 * @brief strarray_dump
 * @param b
 * @param arr
 */
void
strarray_dump(buffer* b, const strarray* arr) {
  char **p = strarray_begin(arr), **e = strarray_end(arr);

  while(p < e) {
    buffer_puts(b, *p);
    buffer_putnlflush(b);
    ++p;
  }
}

#ifdef DEBUG
/**
 * @brief debug_sa
 * @param name
 * @param sa
 */
void
debug_sa(const char* name, stralloc* sa) {
  buffer_puts(buffer_2, name);
  buffer_puts(buffer_2, ": ");
  buffer_putsa(buffer_2, sa);
  buffer_putnlflush(buffer_2);
}

/**
 * @brief debug_s
 * @param name
 * @param s
 */
void
debug_s(const char* name, const char* s) {
  buffer_puts(buffer_2, name);
  buffer_puts(buffer_2, ": ");
  buffer_puts(buffer_2, s);
  buffer_putnlflush(buffer_2);
}

/**
 * @brief debug_sl
 * @param name
 * @param l
 */
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
  // debug_sa(name, &tmp);
  stralloc_free(&tmp);
}

/**
 * @brief debug_int
 * @param name
 * @param i
 */
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
 * @brief path_prefix_b Adds a prefix to the specified path
 * @param prefix        Prefix to add
 * @param x             The path buffer
 * @param n             Length of path buffer
 * @param out           Write output here
 */
void
path_prefix_b(const stralloc* prefix, const char* x, size_t n, stralloc* out) {
  if(prefix->len && !stralloc_equals(prefix, ".")) {
    stralloc_cat(out, prefix);

    if(!stralloc_endb(prefix, &pathsep_make, 1))
      stralloc_catc(out, pathsep_make);
  }
  stralloc_catb(out, x, n);
}

/**
 * @brief path_prefix_s Adds a prefix to the specified path
 * @param prefix        Prefix to add
 * @param path          The path string
 * @param out           Write output here
 */
void
path_prefix_s(const stralloc* prefix, const char* path, stralloc* out) {
  path_prefix_b(prefix, path, str_len(path), out);
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
 * @brief path_object  Convert source file name to object file name
 * @param in
 * @param out
 * @return
 */
char*
path_object(const char* in, stralloc* out) {
  path_prefix_b(&workdir.sa, "", 0, out);
  return path_extension(str_basename(in), out, objext);
}

/**
 * @brief path_wildcard  Replaces the path basename (without extensions) with a wildcard
 * @param path           The path to replace
 * @param sa             Write output here
 * @return               Output string
 */
char*
path_wildcard(stralloc* sa, const char* wildchar) {
  const char* x;
  size_t n, e;

  stralloc_nul(sa);

  x = sa->s;

  if(sa->s[(n = str_rchrs(sa->s, "/\\ ", 3))]) {
    x += n;
    if(*x)
      ++x;
  }

  n = x - sa->s;
  e = byte_rchr(x, sa->len - n, '.');

  if(e != sa->len - n) {
    stralloc_remove(sa, n, e);
    stralloc_insertb(sa, wildchar, n, str_len(wildchar));
  }
  stralloc_nul(sa);

  return sa->s;
}

/**
 * @}
 */

/**
 * @brief extract_build_type
 * @param s
 * @return
 */
int
extract_build_type(const stralloc* s) {
  int i;

  for(i = 0; i < sizeof(build_types) / sizeof(build_types[0]); ++i) {
    if(stralloc_contains(s, build_types[i]))
      return i;
  }
  return -1;
}

/**
 * @brief format_linklib_lib  Output library name (+".lib")
 * @param libname
 * @param out
 */
void
format_linklib_lib(const char* libname, stralloc* out) {
  stralloc_cats(out, libpfx);
  stralloc_cats(out, libname);
  stralloc_cats(out, libext);
}

/**
 * @brief format_linklib_switch  Output library name (+ leading "-l")
 * @param libname
 * @param out
 */
void
format_linklib_switch(const char* libname, stralloc* out) {
  stralloc_cats(out, "-l");
  stralloc_cats(out, libname);

  stralloc_replaces(out, "lib", "");
}

/**
 * @brief format_linklib_dummy
 * @param libname
 * @param out
 */
void
format_linklib_dummy(const char* libname, stralloc* out) {}

/**
 * @brief scan_main  Checks if the given source file contains a main() function
 * @param x
 * @param n
 * @return
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
 * @brief extract_includes  Extract #include directives
 * @param x
 * @param n
 * @param includes
 * @param sys
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

/**
 * @brief get_includes
 * @param srcfile
 * @param includes
 * @param sys
 * @return
 */
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
 * @brief rule_command_subst  Get rule command with substitutions
 * @param rule
 * @param out
 * @param prereq
 * @param plen
 */
void
rule_command_subst(target* rule, stralloc* out, const char* prereq, size_t plen) {
  size_t i;
  stralloc* in = &rule->recipe;

  for(i = 0; i < in->len; ++i) {
    const char* p = &in->s[i];

    if((shell || batch) && i + 4 <= in->len && *p == '$' && p[1] == '(') {
      size_t vlen;
      stralloc_catc(out, shell ? '$' : '%');
      i += 2;
      vlen = byte_chr(&in->s[i], in->len - i, ')');
      stralloc_catb(out, &in->s[i], vlen);
      if(batch)
        stralloc_catc(out, '%');
      i += vlen;
      continue;
    }

    if(i + 2 <= in->len && *p == '$' && str_chr("@^<|", p[1]) < 4) {
      switch(p[1]) {
        case '@': {
          size_t p = out->len;
          stralloc_cats(out, rule->name);
          byte_replace(&out->s[p], out->len - p, pathsep_args == '/' ? '\\' : '/', pathsep_args);
          break;
        }
        case '^': {
          stralloc_catb(out, prereq, plen);
          break;
        }
        case '|': {
          stralloc_subst(out, prereq, plen, " ", make_sep_inline ? make_sep_inline : "\n ");
          break;
        }
        case '<': {
          size_t n = byte_chr(prereq, plen, ' ');
          stralloc_catb(out, prereq, n);
          break;
        }
      }
      ++i;
    } else {
      if(!stralloc_append(out, p))
        break;
    }
  }
}

/**
 * @brief rule_command
 * @param rule
 * @param out
 */
void
rule_command(target* rule, stralloc* out) {
  size_t len;
  const char* pfx = 0;
  char *s, from = pathsep_args == '/' ? '\\' : '/';
  strlist prereq;
  strlist_init(&prereq, ' ');

  if(stralloc_contains(&rule->recipe, "-+$^")) {
    //    pfx = "-+";
  }

  strlist_foreach(&rule->prereq, s, len) {
    if(pfx) {
      strlist_push(&prereq, pfx);
      stralloc_catb(&prereq.sa, s, len);
    } else {
      strlist_pushb(&prereq, s, len);
    }
  }
  //  stralloc_copy(&prereq.sa, &rule->prereq.sa);

  stralloc_replacec(&prereq.sa, from, pathsep_args);

  if(0) { // make_begin_inline == NULL && rule->recipe == &lib_command) {
    char* x;
    size_t n = 0;
    range r;
    r.start = stralloc_begin(&prereq.sa);
    r.end = stralloc_end(&prereq.sa);
    for(; r.start < r.end;) {
      for(x = r.start;;) {
        n = strlist_skip(&prereq, x);
        if(n == 0 || x + n - r.start > 512)
          break;
        x += n;
      }
      if(out->len) {
        stralloc_cats(out, newline);
        stralloc_catc(out, '\t');
      }

      if(pfx && byte_equal(r.start, str_len(pfx), pfx)) {
        r.start += 2;
      }

      n = x - r.start;

      if(n > 0 && r.start[n - 1] == ' ')
        --n;

      rule_command_subst(rule, out, r.start, n);

      if(r.start + n < r.end && r.start[n] == ' ')
        ++n;

      r.start += n;
    }
  } else if(str_start(make, "g")) {
    stralloc_copy(out, &rule->recipe);
  } else {
    rule_command_subst(rule, out, prereq.sa.s, prereq.sa.len);
  }

  stralloc_free(&prereq.sa);
}

/**
 * @brief subst_var
 * @param in
 * @param out
 * @param pfx
 * @param sfx
 * @param tolower
 */
void
subst_var(const stralloc* in, stralloc* out, const char* pfx, const char* sfx, int tolower) {
  size_t i;
  stralloc_zero(out);
  for(i = 0; i < in->len; ++i) {
    const char* p = &in->s[i];

    if(i + 4 <= in->len && *p == '$' && p[1] == '(') {
      size_t vlen;
      stralloc_cats(out, pfx);
      i += 2;
      vlen = byte_chr(&in->s[i], in->len - i, ')');
      stralloc_catb(out, &in->s[i], vlen);
      if(tolower)
        byte_lower(&out->s[out->len - vlen], vlen);
      stralloc_cats(out, sfx);
      i += vlen;
      continue;
    }

    stralloc_append(out, p);
  }
}

/**
 * Find or create rule
 */
target*
get_rule(const char* name) {
  target* ret = NULL;
  TUPLE* t;
  size_t len = str_len(name);

  if(rules == NULL)
    hmap_init(1024, &rules);

  if(hmap_search(rules, name, len + 1, &t) == HMAP_SUCCESS) {
    ret = t->vals.val_custom;
  } else {
    ret = malloc(sizeof(target));
    byte_zero(ret, sizeof(target));
    // ret->serial = 0;

    hmap_add(&rules, name, len + 1, 0, HMAP_DATA_TYPE_CUSTOM, ret);
    hmap_search(rules, name, len + 1, &t);

    ret->name = t->key;

    strlist_init(&ret->output, ' ');

    ret->output.sa.s = t->key;
    ret->output.sa.len = len;

    strlist_init(&ret->prereq, ' ');

#ifdef DEBUG_OUTPUT
    buffer_putm_internal(buffer_2, "Created rule '", ret->name, "'\n", 0);
    buffer_flush(buffer_2);
#endif
  }

  return ret;
}

/**
 * @brief get_rule_sa
 * @param name
 * @return
 */
target*
get_rule_sa(stralloc* name) {
  stralloc_nul(name);
  return get_rule(name->s);
}

/**
 * @brief find_rule
 * @param needle
 * @return
 */
target*
find_rule(const char* needle) {
  TUPLE* t;

  hmap_foreach(rules, t) {
    const char* name = t->key;

    if(str_equal(name, needle))
      return t->vals.val_custom;

    if(str_equal(path_basename((char*)name), path_basename((char*)needle)))
      return t->vals.val_custom;

    if(t->next == rules->list_tuple)
      break;
  }
  return 0;
}

/**
 * @brief find_rule_sa
 * @param name
 * @return
 */
target*
find_rule_sa(stralloc* name) {
  stralloc_nul(name);
  return find_rule(name->s);
}

/**
 * @brief find_rule_b
 * @param x
 * @param n
 * @return
 */
target*
find_rule_b(const char* x, size_t n) {
  target* r;
  stralloc sa;
  stralloc_init(&sa);
  sa.s = (char*)x;
  sa.len = n;
  r = find_rule_sa(&sa);
  return r;
}

/**
 * @brief match_rule
 * @param rule           Target rule
 * @param pattern        Preqreq wildcard pattern
 * @return               1 if match
 */
int
match_rule(target* rule, const char* pattern) {
  int ret = 0;
  const char* s;
  size_t n;
  stralloc sa;
  stralloc_init(&sa);

  strlist_foreach(&rule->prereq, s, n) {
    stralloc_copyb(&sa, s, n);
    if(str_equal(pattern, path_wildcard(&sa, "%"))) {
      ret = 1;
      break;
    }
  }
  stralloc_free(&sa);
  return ret;
}

/**
 * Add a path to a strlist
 */
void
add_path(strlist* list, const char* path) {
  size_t i, len = str_len(path);

  if(strlist_push_unique(list, path)) {
    for(i = list->sa.len - len; i < list->sa.len; ++i) {
      if(list->sa.s[i] == '/' || list->sa.s[i] == '\\')
        list->sa.s[i] = pathsep_make;
    }
  }
}

/**
 * @brief add_srcpath
 * @param list
 * @param path
 */
void
add_srcpath(strlist* list, const char* path) {
  if(srcdir.len && !stralloc_equals(&srcdir, ".")) {
    stralloc sa;
    stralloc_init(&sa);

    stralloc_copy(&sa, &srcdir);
    if(!stralloc_endb(&srcdir, &pathsep_make, 1))
      stralloc_catc(&sa, pathsep_make);
    stralloc_cats(&sa, path);

    strlist_push_unique_sa(list, &sa);
    stralloc_free(&sa);
  } else {
    strlist_push_unique(list, path);
  }
}

/**
 * @brief add_path_sa
 * @param list
 * @param path
 */
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
 * @brief new_source  Create new source file entry.
 * @param name
 * @return
 */
sourcefile*
new_source(const char* name) {
  sourcefile* ret;

  if((ret = (sourcefile*)malloc(sizeof(sourcefile)))) {
    byte_zero(ret, sizeof(sourcefile));
    ret->name = str_dup(name);
    ret->has_main = has_main(ret->name) == 1;

    return ret;
  }
  return 0;
}

/**
 * @brief add_source  Adds a source file to the given list.
 * @param filename
 * @param sources
 */
void
add_source(const char* filename, strarray* sources) {
  if(str_end(filename, ".c") || str_end(filename, ".h")) {
    strarray_push_unique(sources, filename);
    /*    stralloc sa;
        stralloc_init(&sa);
        stralloc_copys(&sa, filename);
        //    stralloc_replacec(&sa, pathsep_make == '/' ? '\\' : '/', pathsep_make);

        strarray_push_sa_unique(sources, &sa);

        stralloc_free(&sa);*/
  }
}

/**
 * @brief get_sources Searches all source files in the given directory and creates a string-array.
 * @param basedir
 * @param sources
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
 * @brief isset
 * @param name
 * @return
 */
int
isset(const char* name) {
  return hmap_search(vars, name, str_len(name) + 1, 0) == HMAP_SUCCESS;
}

/**
 * @brief get_var  Find or create variable
 * @param name
 * @return
 */
strlist*
get_var(const char* name) {
  TUPLE* t;

  if(vars == NULL)
    hmap_init(1024, &vars);

  if(hmap_search(vars, name, str_len(name) + 1, &t) != HMAP_SUCCESS) {
    strlist var;

    strlist_init(&var, isupper(name[0]) ? ' ' : pathsep_args);

    hmap_set(&vars, name, str_len(name) + 1, &var, sizeof(strlist));
    hmap_search(vars, name, str_len(name) + 1, &t);
  }

  return (strlist*)t->vals.val_chars;
}

/**
 * @brief var
 * @param name
 * @return
 */
const char*
var(const char* name) {
  strlist* v = get_var(name);
  stralloc_nul(&v->sa);
  return v->sa.s;
}

/**
 * @brief set_var  Set variable
 * @param name
 * @param value
 * @return
 */
strlist*
set_var(const char* name, const char* value) {
  strlist* var;
  var = get_var(name);

  stralloc_zero(&var->sa);
  stralloc_copys(&var->sa, value);

  return var;
}

void
unset_var(const char* name) {
  if(isset(name))
    hmap_delete(&vars, (void*)name, str_len(name) + 1);
}

/**
 * @brief push_var  Add value to variable
 * @param name
 * @param value
 */
void
push_var(const char* name, const char* value) {
  strlist* var = get_var(name);

  strlist_push_unique(var, value);
}

/**
 * @brief push_var_sa
 * @param name
 * @param value
 */
void
push_var_sa(const char* name, stralloc* value) {
  strlist_push_unique_sa(get_var(name), value);
}

/**
 * @brief push_lib  Add library spec to variable
 * @param name
 * @param lib
 */
void
push_lib(const char* name, const char* lib) {
  stralloc sa;
  strlist* var = get_var(name);

  stralloc_init(&sa);

  if(format_linklib_fn) {
    format_linklib_fn(lib, &sa);
    push_var_sa(name, &sa);
  }
  stralloc_free(&sa);
}

/**
 * @brief with_lib
 * @param lib
 */
void
with_lib(const char* lib) {
  stralloc def, lib64;
  stralloc_init(&def);
  stralloc_init(&lib64);
  stralloc_copys(&def, "-DHAVE_");

  if(str_find(lib, "lib") == str_len(lib))
    stralloc_cats(&def, "LIB");

  stralloc_cats(&def, lib);
  stralloc_cats(&def, "=1");
  byte_upper(def.s, def.len);

  push_var_sa("DEFS", &def);

  stralloc_copys(&lib64, lib);
  stralloc_cats(&lib64, "$(L64)");
  stralloc_nul(&lib64);

  push_lib("LIBS", lib64.s);
}

void
include_dir(const char* dir) {
  stralloc arg;
  stralloc_init(&arg);
  path_relative(dir, builddir.sa.s, &arg);
  stralloc_prepends(&arg, "-I");

  push_var_sa("CPPFLAGS", &arg);
}

/**
 * @brief push_define
 * @param def
 */
void
push_define(const char* def) {
  stralloc define;
  stralloc_init(&define);
  stralloc_copys(&define, "-D");
  stralloc_cats(&define, def);
  stralloc_nul(&define);

  push_var_sa("DEFS", &define);
}

/**
 * @}
 */

/**
 * @brief get_rules_by_cmd  Search rules by command
 * @param cmd
 * @param deps
 */
void
get_rules_by_cmd(stralloc* cmd, strlist* deps) {
  TUPLE* t;

  hmap_foreach(rules, t) {
    target* rule = t->vals.val_custom;

    if(rule->recipe.s == cmd->s || stralloc_equal(&rule->recipe, cmd)) {
      strlist_push(deps, rule->name);
    }
  }
}

/**
 * @brief dirname_alloc  Gets directory name from a file path (allocated).
 * @param p
 * @return
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
 * @brief populate_sourcedirs  Creates a hash-map of all source directories
 * @param sources
 * @param sourcedirs
 */
void
populate_sourcedirs(strarray* sources, HMAP_DB* sourcedirs) {
  char** srcfile;
  stralloc dir;
  stralloc_init(&dir);

  strarray_foreach(sources, srcfile) {
    size_t n;
    char* x;

    if((x = mmap_read(*srcfile, &n)) != 0) {
      const char* s;
      size_t dlen;
      sourcedir* srcdir;
      sourcefile* file = new_source(*srcfile);
      stralloc r;
      strlist l;
      stralloc_init(&r);
      strlist_init(&l, '\0');

      path_dirname(*srcfile, &dir);
      dlen = dir.len;

      // debug_sa("path_dirname(*srcfile)", &dir);

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

      extract_includes(x, n, &l, 0);

      stralloc_replacec(&l.sa, PATHSEP_C == '\\' ? '/' : '\\', PATHSEP_C);

      strlist_foreach_s(&l, s) {
        dir.len = dlen;
        //

        stralloc_catc(&dir, PATHSEP_C);
        stralloc_cats(&dir, s);
        stralloc_nul(&dir);

        stralloc_zero(&r);
        path_collapse(dir.s, &r);

        //   path_canonical_sa(&dir, &r);

        strlist_push_unique_sa(&srcdir->includes, &r);
      }

      dir.len = dlen;
      // debug_sa("srcdir", &dir);
      // debug_sa("includes", &srcdir->includes);

      stralloc_free(&r);
      strlist_free(&l);

      mmap_unmap(x, n);
    } else {
      buffer_putm_internal(buffer_2, "ERROR opening '", *srcfile, "'\n", 0);
      buffer_putnlflush(buffer_2);
    }
  }

  stralloc_free(&dir);
}

/**
 * @brief dump_sourcedirs
 * @param b
 * @param sourcedirs
 */
void
dump_sourcedirs(buffer* b, HMAP_DB* sourcedirs) {
  TUPLE* t;

  hmap_foreach(sourcedirs, t) {
    sourcedir* srcdir = hmap_data(t);
    sourcefile* pfile;

    buffer_putm_internal(b, "source dir '", t->key, "' (", 0);
    buffer_putulong(b, srcdir->n_sources);
    buffer_puts(b, "): ");
    buffer_put(b, t->key, t->key_len);

    slink_foreach(srcdir->sources, pfile) {
      buffer_putspace(b);
      buffer_puts(b, pfile->name);
    }

    buffer_putnlflush(b);
  }
}

/**
 * @brief includes_to_libs  Include list to library list
 * @param includes
 * @param libs
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
    /*if(stralloc_endb(&sa, "_internal", 9))
      sa.len -= 9;*/

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
static uint32 target_dep_serial;

void
target_dep_list_recursive(strlist* l, target* t, int depth, strlist* hier) {
  target** ptr;

  t->serial = target_dep_serial;

  array_foreach_t(&t->deps, ptr) {
    const char* name = (*ptr)->name;

    if(t->serial == (*ptr)->serial)
      continue;

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

/**
 * @brief target_dep_list
 * @param l
 * @param t
 */
void
target_dep_list(strlist* l, target* t) {

  strlist hier;
  strlist_init(&hier, '\0');
  strlist_push(&hier, t->name);

  strlist_zero(l);

  --target_dep_serial;

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

/**
 * @brief deps_indirect
 * @param l
 * @param names
 */
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

/**
 * @brief deps_direct
 * @param l
 * @param t
 */
void
deps_direct(strlist* l, const target* t) {
  target** ptr;
  array_foreach_t(&t->deps, ptr) {
    if(*ptr) {
      strlist_push(l, (*ptr)->name);
    }
  }
}

/**
 * @brief print_target_deps_r
 * @param b
 * @param t
 * @param deplist
 * @param hierlist
 * @param depth
 */
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
      put_newline(b, 1);

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

/**
 * @brief remove_indirect_deps
 * @param deps
 * @return
 */
ssize_t
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
 * @}
 */

/**
 * @brief deps_for_libs
 * @param rules
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

    stralloc_zero(&sa);
    path_prefix_s(&workdir.sa, str_basename(t->key), &sa);
    stralloc_cats(&sa, libext);

    if((lib = find_rule_sa(&sa))) {
      strlist libs;
      strlist_init(&libs, ' ');

      includes_to_libs(&srcdir->includes, &libs);

      // debug_s("library", lib->name);
      // debug_sl("includes", &srcdir->includes);

      strlist_removes(&libs, lib->name);
      // debug_sl("deps", &libs);

      strlist_zero(&indir);
      deps_indirect(&indir, &libs);

      // debug_sl("indir", &indir);
      strlist_sub(&indir, &libs);

      strlist_sub(&libs, &indir);

// debug_sl("direct", &libs);
#if DEBUG_OUTPUT
      // print_target_deps(buffer_2, lib);
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

/**
 * @brief target_add_dep
 * @param t
 * @param other
 * @return
 */
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

/**
 * @brief target_add_deps
 * @param t
 * @param deps
 */
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
 * @brief gen_clean_rule  Generate clean rule which removes all target outputs
 * @param rules
 */
void
gen_clean_rule(HMAP_DB* rules) {
  target* rule;

  /* Generate "clean" rule */
  if((rule = get_rule("clean"))) {
    TUPLE* t;
    char* arg;
    size_t cmdoffs, lineoffs = 0;
    stralloc fn;
    strlist delete_args;
    stralloc_init(&fn);
    strlist_init(&delete_args, '\0');

    if(delete_command.len == 0)
      stralloc_copys(&delete_command, "DEL /F /Q");

    cmdoffs = delete_command.len;

    hmap_foreach(rules, t) {

      const char* target = t->key;

      /* Ignore the builddir rule */
      if(stralloc_equals(&workdir.sa, t->key))
        continue;

      rule = hmap_data(t);

      if(target[str_chr(target, '%')]) {
        stralloc_nul(&rule->output.sa);
        target = rule->output.sa.s;
      }

      /* If the rule has prerequisites and a recipe, it must be a producing rule */
      if(strlist_count(&rule->prereq) && rule->recipe.s) {
        size_t bpos;

        if(t->key[(bpos = str_rchr(t->key, '{'))]) {
          size_t epos = str_rchr(&t->key[bpos + 1], '}');

          stralloc_zero(&fn);
          stralloc_catb(&fn, &t->key[bpos + 1], epos);
          stralloc_catc(&fn, pathsep_make);
          stralloc_cats(&fn, "*");
          stralloc_catb(&fn, &t->key[bpos + 1 + epos + 1], str_chr(&t->key[bpos + 1 + epos + 1], ':'));
          stralloc_nul(&fn);
          arg = fn.s;
        } else {
          stralloc_copys(&fn, target);

          /* If possible, transform file name into a wildcard pattern */
          arg = path_wildcard(&fn, "*");
        }
        /* Add to deletion list */
        strlist_push_unique(&delete_args, arg);
      }
    }

    strlist_foreach_s(&delete_args, arg) {

      if(delete_command.len - lineoffs + str_len(arg) >= MAX_CMD_LEN) {
        stralloc_readyplus(&delete_command, cmdoffs + 3);
        stralloc_catm_internal(&delete_command, newline, "\t", 0);
        stralloc_catb(&delete_command, delete_command.s, cmdoffs);

        lineoffs = delete_command.len;
      }

      stralloc_catc(&delete_command, ' ');
      stralloc_cats(&delete_command, arg);

      if(arg[str_chr(arg, '*')])
        lineoffs = -MAX_CMD_LEN;
    }

    stralloc_weak(&rule->recipe, &delete_command);
  }
}

/**
 * @brief dump_rule
 * @param rule
 */
void
dump_rule(target* rule) {
#ifdef DEBUG
  buffer_putm_internal(buffer_2, "Rule '", rule->name, "'\n", 0);

  if(rule->prereq.sa.len) {
    stralloc_nul(&rule->prereq.sa);
    buffer_putm_internal(buffer_2, "  prereq: ", rule->prereq.sa.s, "\n", 0);
  }
  if(rule->output.sa.len) {
    stralloc_nul(&rule->output.sa);
    buffer_putm_internal(buffer_2, "  output: ", rule->output.sa.s, "\n", 0);
  }
  if(rule->recipe.len) {
    stralloc_nul(&rule->recipe);
    buffer_putm_internal(buffer_2, "  recipe: ", rule->recipe.s, "\n", 0);
  }
  if(array_length(&rule->deps, sizeof(target*))) {
    target** t;
    buffer_puts(buffer_2, "  deps:");
    array_foreach_t(&rule->deps, t) {
      buffer_putspace(buffer_2);
      buffer_puts(buffer_2, (*t)->name);
    }
    buffer_puts(buffer_2, newline);
  }
  buffer_putnlflush(buffer_2);
#endif
}

/**
 * @brief gen_srcdir_compile_rules  Generate compile rules for every source file given
 * @param rules                     All rules
 * @param srcdir                    source dir structure
 * @param dir                       source dir path
 * @return
 */
target*
gen_srcdir_compile_rules(HMAP_DB* rules, sourcedir* sdir, const char* dir) {
  sourcefile* src;
  target* rule;
  stralloc target, srcs, obj;
  size_t len;

  stralloc_init(&target);
  path_object("%", &target);
  stralloc_cats(&target, ": ");

  stralloc_init(&srcs);

  len = target.len;

  stralloc_init(&obj);

  slink_foreach(&sdir->sources, src) {
    const char *s, *ext;

    if(!str_end(src->name, ".c"))
      continue;

    s = str_basename(src->name);

    target.len = len;

    stralloc_zero(&srcs);
    path_prefix_s(&srcdir, src->name, &srcs);
    path_wildcard(&srcs, "%");
    stralloc_replacec(&srcs, pathsep_make == '/' ? '\\' : '/', pathsep_make);

    stralloc_zero(&obj);
    path_object(src->name, &obj);

    if(str_start(make, "g") || ((shell | batch) && batchmode)) {
      stralloc_cat(&target, &srcs);
      // path_wildcard(&target, "%");
    } else if(batchmode) {
      stralloc_zero(&target);
      stralloc_catm_internal(&target, "{", dir, "}", ext, "{", workdir.sa.s, "}", objext, ":", 0);
    } else {
    }
    stralloc_replacec(&target, pathsep_make == '/' ? '\\' : '/', pathsep_make);

    if((rule = get_rule_sa(&target))) {

      if(rule->recipe.s == 0) {
        strlist_zero(&rule->output);
        strlist_zero(&rule->prereq);
      }

      add_path_sa(&rule->output, &obj);

      // add_srcpath(&rule->prereq, srcs.s);

      if(rule->recipe.s)
        continue;

      if((shell | batch) == 0 && batchmode) {
        // rule->recipe = malloc(sizeof(stralloc));
        stralloc_init(&rule->recipe);
        stralloc_copy(&rule->recipe, &compile_command);
        stralloc_replaces(&rule->recipe, "-Fo", "-Fd");
        stralloc_replaces(&rule->recipe, "$@", workdir.sa.s);

      } else {
        size_t p, e;
        char* x;

        if(workdir.sa.len == 0 || stralloc_equals(&workdir.sa, ".")) {
          stralloc_copy(&rule->recipe, &compile_command);
          x = stralloc_begin(&rule->recipe);
          // e = stralloc_end(&rule->recipe);

          p = e = stralloc_finds(&rule->recipe, "$@");
          while(p > 0 && !((x[p + 1] == '/' || x[p + 1] == '-') && x[p] == ' ')) --p;

          e += 2;
          if(x[e] == '"')
            ++e;

          stralloc_remove(&rule->recipe, p, e - p);
        } else {
          stralloc_weak(&rule->recipe, &compile_command);
        }

        //        rule->recipe = /* str_start(make, "g") ? NULL :*/ &compile_command;
      }
    }
  }

  stralloc_free(&obj);
  stralloc_free(&target);

#ifdef DEBUG_OUTPUT
  dump_rule(rule);
#endif

  return rule;
}

/**
 * @brief gen_simple_compile_rules  Generate compile rules for every source file in srcdir
 * @param rules                     All rules
 * @param srcdir                    source dir structure
 * @param dir                       source dir path
 * @return
 */
target*
gen_simple_compile_rules(HMAP_DB* rules, sourcedir* srcdir, const char* dir) {
  sourcefile* src;
  stralloc obj;
  stralloc_init(&obj);

  slink_foreach(&srcdir->sources, src) {
    target* rule;
    const char* base;

    if(src->name == 0)
      continue;

    base = str_basename(src->name);

    if(!str_equal(base + str_rchr(base, '.'), ".c"))
      continue;

    stralloc_zero(&obj);
    path_object(base, &obj);

    if((rule = get_rule_sa(&obj))) {
      add_srcpath(&rule->prereq, src->name);

      if(rule->recipe.s == NULL) {
        stralloc_weak(&rule->recipe, &compile_command);

        array_catb(&srcdir->rules, &rule, sizeof(target*));
      }
    }
  }

  stralloc_free(&obj);
  return 0;
}

/**
 * @brief lib_rule_for_sourcedir  Generate lib rule for source dir
 * @param rules                     All rules
 * @param srcdir                    source dir structure
 * @param dir                       source dir path
 * @return
 */
target*
lib_rule_for_sourcedir(HMAP_DB* rules, sourcedir* srcdir, const char* name) {
  target *dep = 0, *rule;
  stralloc sa;
  stralloc_init(&sa);

  path_prefix_s(&workdir.sa, name, &sa);
  // stralloc_copys(&sa, name);

  stralloc_cats(&sa, libext);

  // debug_sa("lib_rule_for_sourcedir", &sa);

  if(str_start(make, "g") || batchmode)
    dep = gen_srcdir_compile_rules(rules, srcdir, name);
  else
    dep = gen_simple_compile_rules(rules, srcdir, name);

  if((rule = get_rule_sa(&sa))) {
    sourcefile* pfile;
    strlist_init(&rule->prereq, ' ');

    if(dep) {
      size_t n;
      char* s;
      strlist_cat(&rule->prereq, &dep->output);
      /*      strlist_foreach(&dep->output, s, n) {
              stralloc_zero(&sa);
              path_object(s, &sa);
              add_path_sa(&rule->prereq, &sa);
            }
      */

      // strlist_push(&rule->prereq, dep->name);

      array_catb(&rule->objs, &dep, sizeof(target*));

    } else {
      slink_foreach(srcdir->sources, pfile) {
        if(!str_end(pfile->name, ".c"))
          continue;
        stralloc_zero(&sa);
        path_object(pfile->name, &sa);
        add_path_sa(&rule->prereq, &sa);
      }
    }

    stralloc_weak(&rule->recipe, &lib_command);

#ifdef DEBUG_OUTPUT
    dump_rule(rule);
#endif
  }

  stralloc_free(&sa);
  return rule;
}

/**
 * @brief gen_srcdir_rule
 * @param rules                     All rules
 * @param srcdir                    source dir structure
 * @param name                      source dir name
 */
void
gen_srcdir_rule(HMAP_DB* rules, sourcedir* sdir, const char* name) {
  sourcefile* src;
  target* rule;
  stralloc mask;
  stralloc_init(&mask);

  slink_foreach(&sdir->sources, src) {

    const char* s;
    debug_s("sourcefile", src->name);

    if(!str_end(src->name, ".c"))
      continue;

    s = str_ndup(src->name, str_rchr(src->name, '.'));

    stralloc_zero(&mask);
    path_prefix_s(&workdir.sa, str_basename(s), &mask);
    stralloc_cats(&mask, objext);

    if(batchmode || str_start(make, "g")) {
      stralloc_cats(&mask, ": ");
      stralloc_cats(&mask, src->name);

      path_wildcard(&mask, "%");
      debug_sa("mask", &mask);
    }

    if((rule = get_rule_sa(&mask))) {
      strlist_push(&rule->prereq, src->name);

      if(rule->recipe.s == 0) {
        stralloc_weak(&rule->recipe, &compile_command);
      }
    }

    free((void*)s);
  }
  stralloc_free(&mask);
}

/**
 * @brief gen_lib_rules  Generate library rules for every source dir
 * @param rules
 * @param srcdirs
 */
void
gen_lib_rules(HMAP_DB* rules, HMAP_DB* srcdirs) {
  TUPLE* t;
  stralloc inc;
  stralloc_init(&inc);

  hmap_foreach(srcdirs, t) {
    target* rule;
    sourcedir* srcdir = hmap_data(t);
    const char *s, *base = path_basename(t->key);
    size_t n;

    // debug_s("srcdir", t->key);
    // debug_s("base", base);

    if(strlist_contains(&build_as_lib, base) || str_equal(base, "lib") || base[0] == '.' || base[0] == '\0')
      continue;

    // gen_srcdir_rule(rules, srcdir, base);

    rule = lib_rule_for_sourcedir(rules, srcdir, base);

    array_catb(&srcdir->rules, &rule, sizeof(target*));
  }
  stralloc_free(&inc);
}

/**
 * @brief gen_link_rules  Generate compile rules for every source file with a main()
 * @param rules
 * @param sources
 * @return
 */
int
gen_link_rules(HMAP_DB* rules, strarray* sources) {
  int count = 0;
  target* all;
  const char* x;
  char** srcfile;
  strlist incs, libs, deps, indir;
  stralloc dir, obj, bin;

  strlist_init(&incs, ' ');
  strlist_init(&libs, ' ');
  strlist_init(&deps, ' ');
  strlist_init(&indir, ' ');
  stralloc_init(&dir);
  stralloc_init(&obj);
  stralloc_init(&bin);
  all = get_rule("all");

  strarray_foreach(sources, srcfile) {
    target *compile, *link;
    sourcedir* srcdir;

    strlist_zero(&incs);
    strlist_zero(&libs);
    strlist_zero(&deps);
    strlist_zero(&indir);

    stralloc_zero(&obj);

    if(has_main(*srcfile) == 1) {

      path_dirname(*srcfile, &dir);

      srcdir = get_sourcedir_sa(&dir);

      //      gen_compile_rules(rules, srcdir, dir.s);

      path_object(*srcfile, &obj);

      if((compile = get_rule_sa(&obj))) {

        get_includes(*srcfile, &incs, 0);

        add_srcpath(&compile->prereq, *srcfile);

        stralloc_weak(&compile->recipe, &compile_command);

        /*        stralloc_nul(&incs);
                buffer_putm_internal(buffer_2, "rule '", compile->name, "' includes: ", incs.sa.s, 0);
                buffer_putnlflush(buffer_2);
        */
      }

      stralloc_zero(&bin);
      path_extension(obj.s, &bin, binext);

      add_path_sa(&all->prereq, &bin);

      if((link = get_rule_sa(&bin))) {
        int nremoved;
        sourcefile* pfile;

        add_path_sa(&link->prereq, &obj);

        slink_foreach(srcdir->sources, pfile) {
          if(!pfile->has_main) {
            stralloc_zero(&obj);
            path_object(pfile->name, &obj);

            get_includes(pfile->name, &incs, 0);

            add_path_sa(&link->prereq, &obj);
          }
        }

        //  get_rules_by_cmd(&lib_command, &link->prereq);

        stralloc_weak(&link->recipe, &link_command);

        // debug_s("program", link->name);
        // debug_sa("program libs", &libs.sa);

        /*        deps_indirect(&indir, &libs);

                strlist_sub(&libs, &indir);

                target_add_deps(link, &libs);

                strlist_zero(&deps);
                target_dep_list(&deps, link);

                //debug_sa("final deps", &deps);
        */

        /*
        strlist_zero(&deps);
        strlist_cat(&deps, &libs);

        deps_direct(&deps, link);

        strlist_sub(&deps, &indir);
             //debug_sa("direct deps", &deps);

        array_trunc(&link->deps);

        */
        includes_to_libs(&incs, &libs);

        target_ptrs(&libs, &link->deps);

        strlist_zero(&deps);
        target_dep_list(&deps, link);

        if(strlist_count(&deps))
          strlist_cat(&link->prereq, &deps);

#if 0 // def DEBUG_OUTPUT
        /*print_target_deps(buffer_2, link);
        buffer_putm_internal(buffer_2, "Deps for executable '", link->name, "': ", 0);
        buffer_putsa(buffer_2, &deps.sa);
        buffer_putnlflush(buffer_2);*/
#endif

        ++count;
      }
    }
  }

  strlist_free(&incs);
  strlist_free(&libs);
  strlist_free(&deps);
  strlist_free(&indir);
  stralloc_free(&bin);
  stralloc_free(&obj);
  stralloc_free(&dir);
  return count;
}

/**
 * @brief output_build_rules
 * @param b
 * @param name
 * @param cmd
 */
void
output_build_rules(buffer* b, const char* name, const stralloc* cmd) {
  stralloc out;
  stralloc_init(&out);

  buffer_putm_internal(b, "rule ", name, "\n  command = ", 0);
  subst_var(cmd, &out, "$", "", 1);
  stralloc_replaces(&out, "$@", "$out");
  stralloc_replaces(&out, "$<", "$in");
  stralloc_replaces(&out, "$^", "$in");
  stralloc_remove_all(&out, "\"", 1);
  stralloc_removesuffixs(&out, newline);
  stralloc_removesuffixs(&out, "\r");
  buffer_putsa(b, &out);
  buffer_putsflush(b, newline);
}

/**
 * @brief gen_install_rules
 * @param rules
 * @return
 */
target*
gen_install_rules(HMAP_DB* rules) {
  TUPLE* t;
  target* inst = NULL;
  const char* v = 0;

  hmap_foreach(rules, t) {
    target* rule = t->vals.val_custom;
    int do_lib, do_bin;

    do_lib = (inst_libs && (str_end(t->key, ".lib") || str_end(t->key, ".a") || t->key[str_find(t->key, ".so")] ||
                            rule->recipe.s == lib_command.s));

    do_bin = (inst_bins && (str_end(t->key, ".dll") || str_end(t->key, ".exe") || rule->recipe.s == link_command.s));

    if(!(do_lib || do_bin))
      continue;

    if(!inst) {
      inst = get_rule("install");

      // inst->recipe = malloc(sizeof(stralloc));
      stralloc_init(&inst->recipe);

      strlist_push(&inst->prereq, "all");
    }

    if(!isset("prefix")) {

      set_var("prefix", "/usr");
      stralloc_catm_internal(&inst->recipe, newline, "\t$(INSTALL_DIR) $(DESTDIR)$(prefix)", 0);

      if(!v) {
        v = set_var("INSTALL", "install")->sa.s;

        set_var("INSTALL_DIR", str_start(v, "install") ? "$(INSTALL) -d" : "mkdir");

        if(do_lib)
          set_var("INSTALL_DATA", str_start(v, "install") ? "$(INSTALL) -m 644" : "$(INSTALL)");

      }
    }

    if(do_bin) {
      if(!isset("bindir")) {
        set_var("bindir", "$(prefix)/bin");
        stralloc_catm_internal(&inst->recipe, newline, "\t$(INSTALL_DIR) $(DESTDIR)$(bindir)", 0);
      }
      
      set_var("INSTALL_EXEC", str_start(v, "install") ? "$(INSTALL) -m 755" : "$(INSTALL)");

      stralloc_catm_internal(&inst->recipe, newline, "\t$(INSTALL_EXEC) ", t->key, " $(DESTDIR)$(bindir)", 0);
    }

    if(do_lib) {
      if(!isset("libdir")) {
        set_var("libdir", "$(prefix)/lib");
        if(str_end(compiler, "64")) {
          push_var("libdir", "$(X64)");
        }

        stralloc_catm_internal(&inst->recipe, newline, "\t$(INSTALL_DIR) $(DESTDIR)$(libdir)", 0);
      }

      stralloc_catm_internal(&inst->recipe, newline, "\t$(INSTALL_DATA) ", t->key, " $(DESTDIR)$(libdir)", 0);
    }
  }
  return inst;
}

/**
 * @brief output_all_vars  Output all variables
 * @param b
 * @param vars
 */
void
output_all_vars(buffer* b, HMAP_DB* vars) {
  stralloc v;
  TUPLE* t;
  stralloc_init(&v);

  hmap_foreach(vars, t) {
    strlist* var = hmap_data(t);

    if(var->sa.len) {
      stralloc_copys(&v, t->key);
      if(ninja)
        stralloc_lower(&v);

      stralloc_nul(&v);

      if(batch)
        buffer_putm_internal(b, "@SET ", v.s, "=", 0);
      else if(shell)
        buffer_putm_internal(b, v.s, "=\"", 0);
      else
        buffer_putm_internal(b, v.s, " = ", 0);

      if(ninja || shell) {
        stralloc_zero(&v);
        subst_var(&var->sa, &v, "$", "", 1);
        buffer_putsa(b, &v);
      } else if(batch) {
        stralloc_zero(&v);
        subst_var(&var->sa, &v, "%", "%", 1);
        buffer_putsa(b, &v);
      } else {
        buffer_putsa(b, &var->sa);
      }

      if(shell)
        buffer_putc(b, '"');

      put_newline(b, 0);
    }
  }
  put_newline(b, 1);
}

/**
 * @brief output_make_rule  Output rule to buffer
 * @param b
 * @param rule
 */
void
output_make_rule(buffer* b, target* rule) {
  size_t num_deps = strlist_count(&rule->prereq);

  /* if(array_length(&rule->deps, sizeof(target*)))
     print_target_deps(b, rule);
  */

  if(num_deps == 0 && str_diffn(rule->name, workdir.sa.s, workdir.sa.len) &&
     !rule->name[str_chr(rule->name, pathsep_make)] && str_end(rule->name, ":")) {
    buffer_putm_internal(b, ".PHONY: ", rule->name, newline, 0);
  }

  if(!stralloc_equals(&rule->output.sa, rule->name)) {
    buffer_putsa(b, &rule->output.sa);
    buffer_puts(b, ": ");
  }
  /*  if(rule->name[str_chr(rule->name, '%')]) {
    stralloc prefix;
    stralloc_init(&prefix);
    if(str_end(rule->name, ".c")) {
      const char* s;
      size_t n;
      strlist_foreach(&rule->prereq, s, n) {
        size_t l;
        if(prefix.len)
          stralloc_cats(&prefix, " \\\n");

        if((l = byte_rchr(s, n, pathsep_make)) < n) {
          s += l + 1, n -= l + 1;
        }
        path_prefix_b(&workdir.sa, s, byte_rchr(s, n, '.'), &prefix);
        stralloc_cats(&prefix, objext);
      }
      stralloc_cats(&prefix, ": ");

      path_prefix_s(&workdir.sa, path_basename(rule->name), &prefix);
      prefix.len = byte_rchr(prefix.s, prefix.len, '.');
      stralloc_cats(&prefix, objext);
      stralloc_cats(&prefix, ": ");
      buffer_putsa(b, &prefix);
      num_deps = 0;
    }
  }*/ /*else {
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            */
  buffer_puts(b, rule->name);

  if(!rule->name[str_chr(rule->name, '%')])
    buffer_putc(b, ':');
  /*  }
   */
  if(num_deps) {
    stralloc prereq;
    stralloc_init(&prereq);
    stralloc_copy(&prereq, &rule->prereq.sa);
    stralloc_replacec(&prereq, pathsep_make == '/' ? '\\' : '/', pathsep_make);

    if(!str_end(rule->name, ":")) {
      buffer_putspace(b);
      buffer_putsa(b, &prereq);
    }
    stralloc_free(&prereq);
  }

  if(&rule->recipe) {
    stralloc cmd;
    stralloc_init(&cmd);

    rule_command(rule, &cmd);

    if(!stralloc_starts(&cmd, newline)) {
      put_newline(b, 0);
      buffer_putc(b, '\t');
    }
    buffer_putsa(b, &cmd);

    put_newline(b, 0);

    if(str_end(rule->name, ":")) {
      put_newline(b, 0);
      put_newline(b, 0);
      buffer_putsa(b, &rule->prereq.sa);
      buffer_puts(b, " :");
      put_newline(b, 0);
    }
    stralloc_free(&cmd);
  }

  put_newline(b, 1);
}

/**
 * @brief gen_clean_rule
 * @param rules
 */
/**
 * @brief output_ninja_rule
 * @param b
 * @param rule
 */
void
output_ninja_rule(buffer* b, target* rule) {
  const char* rule_name = 0;

  if(rule->recipe.s == compile_command.s)
    rule_name = "cc";
  else if(rule->recipe.s == link_command.s)
    rule_name = "link";
  else if(rule->recipe.s == lib_command.s)
    rule_name = "lib";

  if(rule_name) {
    stralloc path;
    stralloc_init(&path);
    stralloc_subst(
        &path, rule->name, str_len(rule->name), pathsep_args == '/' ? "\\" : "/", pathsep_args == '/' ? "/" : "\\");

    buffer_puts(b, "build ");
    buffer_putsa(b, &path);
    buffer_puts(b, ": ");
    buffer_puts(b, rule_name);
    buffer_puts(b, " ");

    stralloc_zero(&path);
    stralloc_subst(&path,
                   rule->prereq.sa.s,
                   rule->prereq.sa.len,
                   pathsep_args == '/' ? "\\" : "/",
                   pathsep_args == '/' ? "/" : "\\");

    buffer_putsa(b, &path);

    buffer_putnlflush(b);
    stralloc_free(&path);
  }
}

/**
 * @brief output_all_rules  Output the rule set
 * @param b
 * @param hmap
 */
void
output_all_rules(buffer* b, HMAP_DB* hmap) {
  TUPLE* t;

  hmap_foreach(hmap, t) {
    target* rule = t->vals.val_custom;
    const char* name = t->key;

#ifdef DEBUG_OUTPUT
    buffer_putm_internal(buffer_2, "Outputting rule '", name, "'", newline, 0);
    buffer_flush(buffer_2);
#endif

    if(ninja)
      output_ninja_rule(b, t->vals.val_custom);
    else
      output_make_rule(b, t->vals.val_custom);
  }
}

/**
 * @brief output_script
 * @param b
 * @param rule
 */
void
output_script(buffer* b, target* rule) {
  static uint32 serial;
  char* x;
  size_t n;
  int flush = 0;

  if(rule == NULL) {
    TUPLE* t;
    ++serial;

    /*    hmap_foreach(rules, t) {
          rule = hmap_data(t);

          output_script(b, rule);
        }
    */
    flush = 1;
    rule = get_rule("all");
  }

  if(rule->serial == serial)
    return;

  if(!rule->name[str_chr(rule->name, '%')]) {
    if(rule->recipe.s != compile_command.s)
      buffer_putm_internal(b, newline, "REM Rules for '", rule->name, "'", newline, 0);
  }

  strlist_foreach(&rule->prereq, x, n) {
    target* dep = find_rule_b(x, n);

    if(!dep || dep->serial == serial)
      continue;

    output_script(b, dep);
  }

  if(array_length(&rule->objs, sizeof(target*))) {
    target** tptr;
    array_foreach_t(&rule->objs, tptr) {
      target* dep = *tptr;

      if(dep == 0 || dep->serial == serial)
        continue;

      // if(dep->name[str_chr(dep->name, '%')])
      output_script(b, dep);
    }
  }

  if(rule->recipe.len) {
    stralloc cmd;
    stralloc_init(&cmd);
    rule_command(rule, &cmd);
    buffer_putsa(b, &cmd);
    stralloc_free(&cmd);

    buffer_puts(b, " || GOTO FAIL");
  }

  if(str_equal(rule->name, "all")) {
    buffer_putm_internal(b,
                         newline,
                         ":SUCCESS",
                         newline,
                         "ECHO Done.",
                         newline,
                         "GOTO QUIT",
                         newline,
                         newline,
                         ":FAIL",
                         newline,
                         "ECHO Fail.",
                         newline,
                         newline,
                         ":QUIT",
                         newline,
                         0);
  }

  put_newline(b, flush);
  rule->serial = serial;
}

/**
 * @brief set_machine  Set the machine type
 * @param s
 * @return
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
 * @brief set_system  Set the system type
 * @param s
 * @return
 */
int
set_system(const char* s) {

  int ret = 1;

  if(s[str_find(s, "win")]) {
    sys.os = WINDOWS;
    sys.type = WIN;
  } else if(s[str_find(s, "mac")]) {
    sys.os = MAC;
    sys.type = UNIX;
  } else if(s[str_find(s, "lin")]) {
    sys.os = LINUX;
    sys.type = UNIX;
  } else {
    ret = 0;
  }
  
  return ret;
}

/**
 * @brief set_make_type  Set make program type
 * @param make
 * @param compiler
 * @return
 */
int
set_make_type(const char* make, const char* compiler) {
  const char* inst = "install";

#if WINDOWS_NATIVE
  newline = "\r\n";
#else
  newline = "\n";
#endif

  stralloc_copys(&mkdir_command, "IF NOT EXIST \"$@\" MKDIR \"$@\"");

  if(str_start(make, "batch") || str_start(make, "cmd")) {
    pathsep_args = '\\';
    pathsep_make = '\\';

  } else if(str_start(make, "bmake") || str_start(make, "borland")) {

    /* Borland C++ Builder Make */
    pathsep_make = '\\';
    make_begin_inline = "@&&|\r\n ";
    make_sep_inline = " ";
    make_end_inline = "\r\n|";
    inst = "copy /y";

    newline = "\r\n";

  } else if(str_start(make, "nmake")) {

    /* Microsoft NMake */
    pathsep_make = '\\';
    /*    make_begin_inline = "@<<$*.rsp\r\n";
        make_end_inline = "\r\n<<keep";
    */
    newline = "\r\n";

    inst = "copy /y";

  } else if(str_start(make, "gmake") || str_start(make, "gnu")) {

    newline = "\n";
    pathsep_make = '/';
    stralloc_copys(&mkdir_command, "test -d \"$@\" || mkdir -p \"$@\"");
    stralloc_copys(&delete_command, "rm -f");

  } else if(str_start(make, "omake") || str_start(make, "orange")) {
    pathsep_make = '\\';

    if(inst_bins || inst_libs)
      set_var("INSTALL", "copy /y");

  } else if(str_start(compiler, "pelles") || str_start(compiler, "po")) {
    pathsep_make = '\\';

    make_begin_inline = "@<<\r\n ";
    make_end_inline = "\r\n<<";

    inst = "copy /y";

  } else if(str_start(make, "ninja")) {
    ninja = 1;
    pathsep_make = pathsep_args = PATHSEP_C;

    make_begin_inline = make_sep_inline = make_end_inline = 0;
  }

  if(inst_bins || inst_libs)
    set_var("INSTALL", inst);

  pathsep_args = pathsep_make;

  return 1;
}

/**
 * @brief set_compiler_type Set the compiler type
 * @param compiler
 * @return
 */
int
set_compiler_type(const char* compiler) {

  set_var("CC", "cc");
  set_var("CXX", "c++");

  stralloc_copys(&compile_command, "$(CC) $(CFLAGS) $(CPPFLAGS) $(DEFS) -c -o \"$@\" $<");
  set_command(&lib_command, "$(LIB) /out:$@", "$^");
  set_command(&link_command, "$(CC) $(CFLAGS) $(LDFLAGS) -o \"$@\"", "$^ $(LIBS) $(EXTRA_LIBS) $(STDC_LIBS)");

  if(build_type == BUILD_TYPE_DEBUG) {
    push_var("CPPFLAGS", "-DDEBUG=1");
  } else {
    push_var("CPPFLAGS", "-DNDEBUG=1");
    push_var("CFLAGS", build_type == BUILD_TYPE_MINSIZEREL ? "-O1" : "-O2");
  }

  push_var("DEFS", "-DHAVE_ERRNO_H=1");

  /*
   * Visual C++ compiler
   */
  if(str_start(compiler, "msvc") || str_start(compiler, "icl") || str_start(compiler, "vs20") ||
     str_start(compiler, "vc") || compiler[str_find(compiler, "-cl")]) {

    objext = ".obj";
    binext = ".exe";
    libext = ".lib";

    set_var("CC", "cl -nologo");
    set_var("LIB", "lib");
    set_var("LINK", "link");
    push_var("CFLAGS", build_type == BUILD_TYPE_DEBUG ? "-MTd" : "-MT");
    push_var("CPPFLAGS", "-Dinline=__inline");

    if(build_type == BUILD_TYPE_DEBUG || build_type == BUILD_TYPE_RELWITHDEBINFO)
      push_var("CFLAGS", "-Zi");

    if(build_type == BUILD_TYPE_MINSIZEREL)
      push_var("CFLAGS", "-Os");
    else if(build_type != BUILD_TYPE_DEBUG)
      push_var("CFLAGS", "-Ox");
    /*    push_var("LDFLAGS",
                 "/DEBUG /DYNAMICBASE /INCREMENTAL /NXCOMPAT /TLBID:1");
    */
    // push_var("LDFLAGS", "/SUBSYSTEM:CONSOLE /TLBID:1 /DYNAMICBASE /NXCOMPAT");

    //  push_var("LDFLAGS", "/MANIFEST /manifest:embed2 /MANIFESTUAC:\"level=asInvoker uiAccess=false\"");

    stralloc_copys(&compile_command, "$(CC) $(CFLAGS) $(CPPFLAGS) $(DEFS) -c -Fo $@ $<");
    set_command(&lib_command, "$(LIB) -out:$@", "$^");
    //    stralloc_copys(&lib_command, "$(LIB) /OUT:$@ @<<\n\t\t$^\n<<");

    /*
     * Intel C++ compiler
     */
    if(str_start(compiler, "icl")) {
      set_var("CC", "icl -nologo");
      set_var("CXX", "icl -nologo");

      set_var("LINK", "xilink");
      set_var("LIB", "xilib");

      push_var("CFLAGS", "-Qip");
      push_var("CFLAGS", "-Qunroll4");
      push_var("CFLAGS", "-Qauto-ilp32");

      if(mach.bits == _64)
        push_var("LDFLAGS", "-libpath:\"$(ROOT)\\compiler\\lib\\intel64\"");
      else
        push_var("LDFLAGS", "-libpath:\"$(ROOT)\\compiler\\lib\"");

      //      stralloc_copys(&compile_command, "$(CC) $(CFLAGS) $(CPPFLAGS) $(DEFS) -c -Fo $@ $<");
    }

    push_var("LDFLAGS", "-libpath:\"$(UNIVERSALCRTSDKDIR)lib\\$(WINDOWSSDKLIBVERSION)ucrt\\$(MACHINE)\"");
    push_var("LDFLAGS", "-libpath:\"$(WINDOWSSDKDIR)lib\\$(WINDOWSSDKLIBVERSION)um\\$(MACHINE)\"");
    push_var("LDFLAGS", "-libpath:\"$(VCTOOLSINSTALLDIR)lib\\$(MACHINE)\"");

    push_var("LDFLAGS", "-libpath:\"$(WINDOWSSDKDIR)lib$(X64)\"");
    push_var("LDFLAGS", "-libpath:\"$(VCINSTALLDIR)\\lib$(AMD64)\"");
    push_var("LDFLAGS", "-libpath:\"$(VCINSTALLDIR)\\PlatformSDK\\lib$(AMD64)\"");

    push_var("LDFLAGS", "-incremental -manifest");

    if(build_type == BUILD_TYPE_DEBUG)
      push_var("LDFLAGS", "-debug");

    if(str_start(compiler, "icl"))
      push_var("LDFLAGS", "-manifest:embed -manifestuac:\"level='asInvoker' uiAccess='false'\"");

    if(mach.arch == ARM) {
      push_var("LDFLAGS", "-machine:ARM");
      set_var("MACHINE", mach.bits == _64 ? "arm64" : "arm");
    } else if(mach.bits == _64) {
      push_var("LDFLAGS", "-machine:X64");
      set_var("MACHINE", "x64");
      set_var("X64", "\\x64");
      set_var("AMD64", "\\amd64");
    } else if(mach.bits == _32) {
      push_var("LDFLAGS", "-machine:X86");
      set_var("MACHINE", "x86");
      set_var("X64", "");
    }

    set_command(&link_command, "$(LINK) -out:\"$@\" $(LDFLAGS) -pdb:\"$@.pdb\"", "$^ $(LIBS) $(EXTRA_LIBS)");

  } else if(str_start(compiler, "gnu") || str_start(compiler, "gcc") || cygming || str_start(compiler, "clang") ||
            str_start(compiler, "llvm") || str_start(compiler, "zapcc")) {

    libext = ".a";
    objext = ".o";

    if(str_start(compiler, "zapcc"))
      set_var("CC", "zapcc");

    if(build_type == BUILD_TYPE_DEBUG)
      set_var("CFLAGS", "-O0");
    else if(build_type == BUILD_TYPE_MINSIZEREL)
      set_var("CFLAGS", "-Os");
    else
      set_var("CFLAGS", "-O2");

    if(str_end(compiler, "32"))
      push_var("CFLAGS", "-m32");
    if(str_end(compiler, "64"))
      push_var("CFLAGS", "-m64");

    if(build_type == BUILD_TYPE_DEBUG || build_type == BUILD_TYPE_RELWITHDEBINFO) {
      push_var("CFLAGS", "-g");
      push_var("LDFLAGS", "-g");
    }

    /*
     * GNU GCC compatible compilers
     */

    if(str_start(compiler, "gnu") || str_start(compiler, "gcc") || cygming) {
      set_var("CC", "gcc");
      set_var("CXX", "g++");

      set_var("AR", str_start(compiler, "gcc") ? "gcc-ar" : "ar");

      if(build_type == BUILD_TYPE_DEBUG || build_type == BUILD_TYPE_RELWITHDEBINFO)
        push_var("CFLAGS", "-ggdb");

    } else if(str_start(compiler, "clang") || str_start(compiler, "llvm")) {
      pathsep_args = '/';

      set_var("CC", "clang");
      set_var("CXX", "clang++");

      set_var("AR", "llvm-ar");
    }

    set_command(&lib_command, "$(AR) rcs $@", "$^");
    // set_command(&link_command, "$(CC) $(CFLAGS) $(LDFLAGS) -o $@", "$^ $(LIBS) $(EXTRA_LIBS)");
    set_command(&link_command, "$(CC) $(CFLAGS) $(LDFLAGS) -o $@", "$^ $(LIBS)");

    binext = "";

    format_linklib_fn = &format_linklib_switch;

    /*
     * Borland C++ Builder
     */
  } else if(str_start(compiler, "bcc")) {

    pathsep_args = '\\';

    //    push_var("DEFS", "-DWIN32_LEAN_AND_MEAN");
    if(build_type == BUILD_TYPE_MINSIZEREL)
      set_var("CFLAGS", "-O1");
    else if(build_type == BUILD_TYPE_RELEASE || build_type == BUILD_TYPE_RELWITHDEBINFO)
      set_var("CFLAGS", "-O -O2");

    push_var("CFLAGS", "-q -tWC -tWM");
    push_var("CPPFLAGS", "-Dinline=__inline");
    push_var("LDFLAGS", "-q");

    if(build_type == BUILD_TYPE_DEBUG)
      push_var("CFLAGS", "-w");

    if(build_type == BUILD_TYPE_MINSIZEREL)
      push_var("CFLAGS", "-d -a-");

    /* Embracadero C++ */
    if(str_find(compiler, "55") == str_len(compiler) && str_find(compiler, "60") == str_len(compiler)) {
      set_var("CC", "bcc32c");
      set_var("CXX", "bcc32x");

      /* C99 standard */
      push_var("CFLAGS", "-An");

      if(build_type == BUILD_TYPE_DEBUG || build_type == BUILD_TYPE_RELWITHDEBINFO)
        push_var("CFLAGS", "-v");

      /*  if(build_type != BUILD_TYPE_DEBUG)
          push_var("CFLAGS", "-Or");
  */
      set_command(&link_command, "$(CC) $(LDFLAGS) -o $@ ", "$^ $(LIBS) $(EXTRA_LIBS) $(STDC_LIBS)");

      /* Borland C++ Builder 5.5 */
    } else {
      set_var("CC", "bcc32");
      set_var("CXX", "bcc32");

      push_var("CFLAGS", "-ff -fp");

      if(build_type == BUILD_TYPE_DEBUG || build_type == BUILD_TYPE_RELWITHDEBINFO)
        push_var("CFLAGS", "-y");

      if(build_type == BUILD_TYPE_DEBUG || build_type == BUILD_TYPE_RELWITHDEBINFO) {
        push_var("CFLAGS", "-v");
        push_var("LDFLAGS", "-v");
      }

      if(build_type == BUILD_TYPE_DEBUG)
        push_var("CFLAGS", "-w-use");
      else
        push_var("CFLAGS", "-r");

      stralloc_copys(&compile_command, "$(CC) $(CFLAGS) $(CPPFLAGS) $(DEFS) -c -o\"$@\" $<");
      set_command(&link_command, "$(CC) $(LDFLAGS) -e\"$@\"", "$^ $(LIBS) $(EXTRA_LIBS) $(STDC_LIBS)");
    }

    set_var("LINK", "ilink32");
    set_var("LIB", "tlib");

    push_lib("STDC_LIBS", "cw32");
    push_lib("STDC_LIBS", "import32");

    set_command(&lib_command, "$(LIB) /p256 \"$@\" /u", "$^");

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

    if(build_type == BUILD_TYPE_DEBUG)
      push_var("CFLAGS", "-g2");

    make_begin_inline = 0;
    make_end_inline = 0;
    push_var("STDC_LIBS", "oldnames.lib");

    if(mach.bits == _64) {
      push_var("STDC_LIBS", "ccl64.lib");
      push_var("STDC_LIBS", "libc64.lib");
    } else {
      push_var("STDC_LIBS", "libc.lib");
    }

    stralloc_copys(&link_command, "$(CC) $(CFLAGS) $(LDFLAGS) -o \"$@\" $^ $(LIBS) $(EXTRA_LIBS) $(STDC_LIBS)");
    stralloc_copys(&link_command, "$(LINK) $(LDFLAGS) -o \"$@\" $^ $(LIBS) $(EXTRA_LIBS) $(STDC_LIBS)");

    /*
     * Tiny CC compiler
     */
  } else if(str_start(compiler, "tcc")) {

    libext = ".a";
    objext = ".o";
    format_linklib_fn = &format_linklib_switch;

    set_var("CC", "tcc");
    set_var("AR", "$(CC) -ar");

    if(build_type == BUILD_TYPE_DEBUG || build_type == BUILD_TYPE_RELWITHDEBINFO)
      push_var("CFLAGS", "-g");

    push_var("LDFLAGS", "-Wl,-subsystem=console");

    if(build_type == BUILD_TYPE_MINSIZEREL)
      //      push_var("LDFLAGS", "-Wl,-file-alignment=16");

      push_var("CFLAGS", "-Wall");
    push_var("CPPFLAGS", "-D__TCC__=1");

    set_command(&lib_command, "$(AR) rcs $@", "$^");
    set_command(&link_command, "$(CC) $(LDFLAGS) -o $@", "$^ $(LIBS) $(EXTRA_LIBS)");

  } else if(str_start(compiler, "occ") || str_start(compiler, "orange")) {
    set_var("CC", "occ");
    set_var("LIB", "olib");
    set_var("LINK", "olink");

    objext = ".o";
    libext = ".l";

    push_var("CPPFLAGS", "-Dinline=__inline");
    // push_var("LDFLAGS", "/Wcm");
    push_var("CFLAGS", "-C+? +1 -v -E-36 -E-39");

    if(build_type == BUILD_TYPE_DEBUG || build_type == BUILD_TYPE_RELWITHDEBINFO) {
      push_var("CFLAGS", "+v");
      push_var("LDFLAGS", "-v -c+");
    }

    if(build_type == BUILD_TYPE_DEBUG)
      push_var("CFLAGS", "-O-");

    push_var("LDFLAGS", "-T:CON32");

    push_lib("DEFAULT_LIBS", "clwin");
    push_lib("DEFAULT_LIBS", "climp");

    //    stralloc_copys(&compile_command, "$(CC) /! /c $(CFLAGS) $(CPPFLAGS) $(DEFS) -o\"$@\" \"/I;\" $<");
    stralloc_copys(&compile_command, "$(CC) /! /c $(CFLAGS) $(CPPFLAGS) $(DEFS) -o\"$@\" $<");
    set_command(&lib_command, "$(LIB) /! $@", "$^");
    set_command(&link_command, "$(LINK) -c /! $(LDFLAGS) -o\"$@\"", "$^ c0xpe.o $(LIBS) $(DEFAULT_LIBS)");

  } else if(str_start(compiler, "8cc")) {
    libext = ".a";
    objext = ".o";

    set_var("CC", "8cc");

  } else if(str_start(compiler, "dmc") || str_start(compiler, "digitalmars")) {

    pathsep_args = '\\';

    set_var("CC", "dmc");
    set_var("LIB", "lib");

    set_var("CFLAGS", "");

    if(build_type == BUILD_TYPE_DEBUG || build_type == BUILD_TYPE_RELWITHDEBINFO) {
      push_var("CFLAGS", "-g");
      push_var("LDFLAGS", "-g");
    }
    if(build_type == BUILD_TYPE_MINSIZEREL) {
      push_var("CFLAGS", "-a1 -o+space ");
      push_var("LDFLAGS", "-Nc");
    } else if(build_type == BUILD_TYPE_DEBUG) {
      // push_var("CFLAGS", "-o-");
    } else {
      push_var("CFLAGS", "-o");
    }
    //    set_command(&lib_command, "$(LIB) -c $@", "$^");
    set_command(&lib_command, "$(LIB) -c $@", "$^");
    stralloc_copys(&compile_command, "$(CC) $(CFLAGS) $(CPPFLAGS) $(DEFS) -c -o\"$@\" $<");
    set_command(&link_command, "$(CC) $(CFLAGS) $(LDFLAGS) -o\"$@\"", "$^ $(LIBS) $(EXTRA_LIBS) $(STDC_LIBS)");

  } else if(str_start(compiler, "pelles") || str_start(compiler, "po")) {
    set_var("CC", "cc");
    set_var("LINK", "polink");
    set_var("LIB", "polib");

    set_var("TARGET", mach.bits == _64 ? "amd64-coff" : "x86-coff");

    set_var("CFLAGS", "-W0");

    if(build_type != BUILD_TYPE_DEBUG)
      push_var("CFLAGS", "-Ob1");

    // push_var("CFLAGS", "-fp:precise");

    push_var("CFLAGS", "-Ze"); /* Activates Microsoft's extensions to C */
                               // push_var("CFLAGS", "-Zx"); /* Activates Pelle's extensions to C */
    push_var("CFLAGS", "-Go"); /* Accepts 'old' names for C runtime functions */
                               // push_var("CFLAGS", "-Gz"); /* default to __stdcall */

    push_var("CPPFLAGS", "-D__POCC__");

    if(mach.bits == _64) {
      set_var("MACHINE", "AMD64");
      set_var("L64", "64");
      // libext = "64.lib";
      push_var("CPPFLAGS", "-D_M_AMD64");

    } else if(mach.bits == _32) {
      set_var("MACHINE", "X86");
      set_var("L64", "");
      push_var("CPPFLAGS", "-D_M_IX86");
    }

    push_var("CFLAGS", "-T$(TARGET)");
    push_var("LDFLAGS", "-machine:$(MACHINE)");
    push_var("LDFLAGS", "-libpath:\"%PELLESC%\\lib\"");
    push_var("LDFLAGS", "-libpath:\"%PELLESC%\\lib\\win$(L64)\"");

    /*    if(build_type == BUILD_TYPE_MINSIZEREL)
          push_var("CFLAGS", "-Os");

        else*/
    if(build_type == BUILD_TYPE_DEBUG || build_type == BUILD_TYPE_RELWITHDEBINFO) {
      push_var("CFLAGS", "-Zi");
      push_var("LDFLAGS", "-DEBUG");
    }

    stralloc_copys(&compile_command, "$(CC) $(CFLAGS) $(CPPFLAGS) $(DEFS) -c \"$<\" -Fo $@");
    stralloc_copys(&link_command, "$(CC) $^ -Fe $@ $(LDFLAGS) $(LIBS) $(EXTRA_LIBS) $(STDC_LIBS)");

  } else {
    return 0;
  }

  push_lib("EXTRA_LIBS", "advapi32");

  if(str_start(compiler, "dmc"))
    push_lib("EXTRA_LIBS", "wsock32");
  else
    push_lib("EXTRA_LIBS", "ws2_32");

  push_lib("EXTRA_LIBS", "kernel32");

  if(cygming) {

    if(!ninja)
      pathsep_args = '/';

    set_var("prefix", "/");
    push_var("prefix", str_start(toolchain, "mingw") ? toolchain : "usr");

    if(cygming && 0) {
      strlist* cross = set_var("CROSS_COMPILE", str_end(toolchain, "64") ? "x86_64" : "i686");

      //      cross->sep = '-';
      stralloc_cats(&cross->sa, str_start(toolchain, "mingw") ? "-w64-" : "-pc-");
      stralloc_cats(&cross->sa,
                    str_start(toolchain, "mingw") ? "mingw32" : str_start(toolchain, "msys") ? "msys" : "cygwin");

      stralloc_catc(&cross->sa, '-');
    }
  }

  if(isset("CROSS_COMPILE")) {
    stralloc_prepends(&compile_command, "$(CROSS_COMPILE)");
    stralloc_prepends(&lib_command, "$(CROSS_COMPILE)");
    stralloc_prepends(&link_command, "$(CROSS_COMPILE)");
  }

  return 1;
}

/**
 * @brief usage  Show command line usage
 * @param argv0
 */
void
usage(char* argv0) {
  buffer_putm_internal(buffer_1,
                       "Usage: ",
                       str_basename(argv0),
                       " [sources...]\n"
                       "\n"
                       "Options\n"
                       "  -h, --help                show this help\n"
                       "\n"
                       "  -o, --output FILE         write to file\n"
                       "\n"
                       "  -O, --objext EXT          object file extension\n"
                       "  -B, --exeext EXT          binary file extension\n"
                       "  -L, --libext EXT          library file extension\n"
                       "\n"
                       "  -a, --create-libs         create rules for libraries\n"
                       "  -o, --create-objs         create rules for objects\n"
                       "  -b, --create-bins         create rules for programs\n"
                       "  -i, --install             create installation rules\n"
                       "\n"
                       "  -d, --builddir            build directory\n"
                       "  -a, --arch                set architecture\n"
                       "  -s, --system OS           set operating system\n"
                       "  -c, --cross TARGET        set cross compiler\n"
                       "\n"
                       "  -D, --define NAME[=VALUE] add a preprocessor definition\n"
                       "  -I, --includedir DIR      add an include directory\n"
                       "  -l, --link LIB            link a library\n"
                       "\n"
                       "  -t, --compiler-type TYPE  compiler type, one of:\n"
                       "\n"
                       "     gcc         GNU make\n"
                       "     bcc55       Borland C++ Builder 5.5\n"
                       "     bcc         Borland C++ Builder >= 6.0\n"
                       "     lcc         lcc\n"
                       "     tcc         TinyCC\n"
                       "     msvc        Visual C++\n"
                       "     icl         Intel C++\n"
                       "     clang       LLVM\n"
                       "     occ         OrangeC\n"
                       "     dmc         Digital Mars C++\n"
                       "     pocc        Pelles-C\n"
                       "     zapcc[-cl]  ZapCC\n"
                       "\n"
                       "  -m, --make-type TYPE      make program type, one of:\n"
                       "     nmake       Microsoft NMake\n"
                       "     borland     Borland Make\n"
                       "     gmake       GNU Make\n"
                       "     omake       OrangeCC Make\n"
                       "     pomake      Pelles-C Make\n"
                       "     make        Other make\n"
                       "     batch       Windows batch (.bat .cmd)\n"
                       "     ninja       Ninja build\n"
                       "\n"
                       "  --debug        Specify build type"
                       "  --release\n"
                       "  --relwithdebinfo\n"
                       "  --minsizerel\n"
                       "\n",
                       NULL);
  buffer_putnlflush(buffer_1);
}

static stralloc tmp;

/**
 * @brief main
 * @param argc
 * @param argv
 * @return
 */
int
main(int argc, char* argv[]) {
  static int cmd_objs = 0, cmd_libs = 0, cmd_bins = 0;
  int c;
  int ret = 0, index = 0;
  const char *outfile = NULL, *dir = NULL;
  strlist thisdir, outdir, toks;
  strarray args;
  strlist cmdline;
  static strarray libs, includes;
  const char** it;
  const char* s;
  size_t n;

  struct longopt opts[] = {{"help", 0, NULL, 'h'},
                           {"objext", 0, NULL, 'O'},
                           {"exeext", 0, NULL, 'B'},
                           {"libext", 0, NULL, 'X'},
                           {"create-libs", 0, &cmd_libs, 1},
                           {"create-objs", 0, &cmd_objs, 1},
                           {"create-bins", 0, &cmd_bins, 1},
                           {"install", 0, 0, 'i'},
                           {"includedir", 0, 0, 'I'},
                           /*                           {"install-bins", 0, &inst_bins, 1},
                                                     {"install-libs", 0, &inst_libs, 1},*/
                           {"builddir", 0, 0, 'd'},
                           {"compiler-type", 0, 0, 't'},
                           {"make-type", 0, 0, 'm'},
                           {"arch", 0, 0, 'a'},
                           {"system", 0, 0, 's'},
                           {"release", 0, &build_type, BUILD_TYPE_RELEASE},
                           {"relwithdebinfo", 0, &build_type, BUILD_TYPE_RELWITHDEBINFO},
                           {"minsizerel", 0, &build_type, BUILD_TYPE_MINSIZEREL},
                           {"debug", 0, &build_type, BUILD_TYPE_DEBUG},
                           {"define", 0, NULL, 'D'},
                           {"build-as-lib", 0, 0, 'L'},
                           {"cross", 0, 0, 'c'},
                           {0}};

  errmsg_iam(argv[0]);
#ifdef _MSC_VER
  optbuf = buffer_1;
#endif

  strlist_init(&cmdline, ' ');
  strlist_fromv(&cmdline, (const char**)argv, argc);

  for(;;) {
    c = getopt_long(argc, argv, "ho:O:B:L:d:t:m:aD:l:I:c:s:", opts, &index);
    if(c == -1)
      break;
    if(c == 0)
      continue;

    switch(c) {
      case 'h':
        usage(argv[0]);
        ret = 0;
        goto exit;
      case 'c': cross_compile = optarg; break;
      case 'o': outfile = optarg; break;
      case 'O': objext = optarg; break;
      case 'B': binext = optarg; break;
      case 'L': strlist_push(&build_as_lib, optarg); break;
      case 'X': libext = optarg; break;
      case 'd': dir = optarg; break;
      case 't': toolchain = compiler = optarg; break;
      case 'm': make = optarg; break;
      case 'a': set_machine(optarg); break;
      case 'a': set_system(optarg); break;
      case 'l': strarray_push(&libs, optarg); break;
      case 'I': strarray_push(&includes, optarg); break;
      case 'i':
        inst_bins = 1;
        inst_libs = 1;
        break;
      case 'D': push_define(optarg); break;
      default:
        buffer_puts(buffer_2, "No such option '-");
        buffer_putc(buffer_2, c);
        buffer_putsflush(buffer_2, "'\n");
        // usage(argv[0]);
        ret = 1;
        goto exit;
    }
  }

  if(!cmd_bins && !cmd_libs && !cmd_objs) {
    cmd_bins = 1;
    cmd_objs = 1;
    cmd_libs = 1;
  }

  if(inst_bins)
    cmd_bins = 1;
  if(inst_libs)
    cmd_libs = 1;

  if(!format_linklib_fn)
    format_linklib_fn = &format_linklib_lib;

  strlist_init(&thisdir, pathsep_make);
  strlist_init(&outdir, pathsep_make);
  strlist_init(&builddir, pathsep_make);
  strlist_init(&workdir, pathsep_make);

  if(outfile) {
    int fd;
    path_dirname(outfile, &outdir.sa);

    if(stralloc_equals(&outdir.sa, "."))
      stralloc_zero(&outdir.sa);
    else
      stralloc_catc(&outdir.sa, pathsep_make);

    mkdir_components(&outdir, 0755);

    if((fd = open_trunc(outfile)) == -1) {
      errmsg_warnsys("ERROR: opening '", outfile, "'", 0);
      ret = 2;
      goto exit;
    }
    buffer_1->fd = fd;

    //  path_absolute_sa(&outdir.sa);
  }

  if(dir) {
    stralloc_copys(&builddir.sa, dir);
  } else if(outdir.sa.len) {
    stralloc_copy(&builddir.sa, &outdir.sa);
  }

  path_getcwd(&thisdir.sa);

  if(build_type == -1) {
    if((build_type = extract_build_type(&builddir.sa)) == -1)
      if((build_type = extract_build_type(&thisdir.sa)) == -1)
        build_type = extract_build_type(&outdir.sa);
  }
  if(build_type == -1)
    build_type = BUILD_TYPE_DEBUG;

  if(make == NULL && compiler) {
    if(str_start(compiler, "b"))
      make = "borland";
    else if(str_start(compiler, "msvc"))
      make = "nmake";
    else if(str_start(compiler, "g"))
      make = "gmake";
    else if(str_start(compiler, "o"))
      make = "omake";
    else if(str_start(compiler, "po"))
      make = "pomake";
  }

  if(toolchain)
    cygming = str_start(toolchain, "mingw") || str_start(toolchain, "cyg") || str_start(toolchain, "msys");

  if(cygming) {
    compiler = "gcc";

    if(make == 0)
      make = "gmake";
  }

  if(make == NULL)
    make = "make";

  batch = str_start(make, "bat") || str_start(make, "cmd");
  ninja = make[str_find(make, "ninja")] != '\0';
  shell = str_start(make, "sh");

  if(batch)
    comment = "REM ";

  if(compiler == NULL)
    compiler = "gcc";
  else if(mach.bits == 0)
    set_machine(compiler);

  debug_sa("builddir", &builddir.sa);

  strlist_init(&toks, '\0');
  strlist_foreach(&builddir, s, n) { strlist_pushb_unique(&toks, s, n); }
  if(outfile) {
    stralloc_catb(&toks.sa, "\0", 1);
    stralloc_cats(&toks.sa, outfile);
  }
  stralloc_replacec(&toks.sa, '/', '\0');
  stralloc_replacec(&toks.sa, '-', '\0');
  stralloc_replacec(&toks.sa, '-', '\0');

  strlist_foreach_s(&toks, s) {
    int i;
    if(set_compiler_type(s)) {
      compiler = s;
      break;
    }
    if(build_type == -1) {
      for(i = 0; i < (sizeof(build_types) / sizeof(build_types[0])); ++i) {
        if(s[case_find(s, build_types[i])]) {
          build_type = i;
          break;
        }
      }
    }
  }

  if(!set_make_type(make, compiler) || !set_compiler_type(compiler)) {
    usage(argv[0]);
    ret = 2;
    goto exit;
  }

  if(*cross_compile) {
    set_var("CROSS_COMPILE", cross_compile);

    if(isset("CC"))
      stralloc_prepends(&get_var("CC")->sa, "$(CROSS_COMPILE)");
    if(isset("CXX"))
      stralloc_prepends(&get_var("CXX")->sa, "$(CROSS_COMPILE)");
    if(isset("AR"))
      stralloc_prepends(&get_var("AR")->sa, "$(CROSS_COMPILE)");
  }

  batchmode = batch && stralloc_contains(&compile_command, "-Fo");

  if(batch)
    pathsep_args = pathsep_make;

  strarray_foreach(&libs, it) { with_lib(*it); }
  strarray_foreach(&includes, it) { include_dir(*it); }

  stralloc_replacec(&outdir.sa, PATHSEP_C == '/' ? '\\' : '/', PATHSEP_C);
  //  path_absolute_sa(&outdir.sa);

  stralloc_nul(&outdir.sa);
  stralloc_nul(&thisdir.sa);

  if(builddir.sa.len == 0) {
    if(strlist_contains(&outdir, "build")) {
      stralloc_copy(&builddir.sa, &outdir.sa);
      // path_relative(outdir.sa.s, thisdir.sa.s, &builddir.sa);
    } else if(toolchain && !strlist_contains(&thisdir, "build")) {
      stralloc_copy(&builddir.sa, &thisdir.sa);
      strlist_push(&builddir, dir ? dir : "build");
      strlist_push(&builddir, toolchain);
      strlist_push(&builddir, build_types[build_type]);
    }

    stralloc_replacec(&builddir.sa, PATHSEP_C == '/' ? '\\' : '/', PATHSEP_C);
  }

  path_absolute_sa(&outdir.sa);
  path_absolute_sa(&builddir.sa);

  stralloc_nul(&thisdir.sa);
  stralloc_nul(&outdir.sa);
  stralloc_nul(&builddir.sa);

  debug_sa("thisdir", &thisdir.sa);
  debug_sa("outdir", &outdir.sa);
  debug_sa("builddir", &builddir.sa);
  /*
    stralloc_nul(&outdir.sa);
    stralloc_nul(&thisdir.sa);
    path_relative(outdir.sa.s, thisdir.sa.s, &outdir.sa);
  */

  path_relative(builddir.sa.s, outdir.sa.s, &workdir.sa);
  stralloc_nul(&workdir.sa);
  stralloc_nul(&workdir.sa);
  debug_sa("workdir", &workdir.sa);

  stralloc_nul(&thisdir.sa);
  stralloc_nul(&outdir.sa);
  path_relative(thisdir.sa.s, outdir.sa.s, &srcdir);
  stralloc_nul(&srcdir);
  debug_sa("srcdir", &srcdir);

  if(outdir.sa.len) {
    stralloc_replacec(&thisdir.sa, PATHSEP_C == '/' ? '\\' : '/', PATHSEP_C);
    stralloc_replacec(&outdir.sa, PATHSEP_C == '/' ? '\\' : '/', PATHSEP_C);

    // debug_sa("thisdir", &thisdir.sa);
    // debug_sa("outdir", &outdir.sa);

    path_absolute_sa(&outdir.sa);
    stralloc_zero(&tmp);
    path_relative(thisdir.sa.s, outdir.sa.s, &tmp);

    // if(tmp.len) {
    stralloc_copy(&srcdir, &tmp);
    // debug_sa("srcdir", &srcdir);
    //}
    stralloc_zero(&tmp);
  }

  // debug_sa("srcdir", &srcdir);

  path_relative(builddir.sa.s, outdir.sa.s, &tmp);
  debug_sa("tmp", &tmp);

  stralloc_replacec(&workdir.sa, pathsep_make == '/' ? '\\' : '/', pathsep_make);

  /*
    if(tmp.len) {
      stralloc_catc(&tmp, pathsep_make);
      stralloc_copy(&builddir.sa, &tmp);
    }
    stralloc_free(&tmp);

    //debug_sa("builddir", &builddir.sa);
  */
  strarray_init(&args);
  strarray_init(&srcs);

  while(optind < argc) {
    stralloc arg;
    stralloc_init(&arg);
    stralloc_copys(&arg, argv[optind]);
    stralloc_nul(&arg);

    if(stralloc_contains(&arg, "=")) {
      size_t eqpos;
      const char* v;
      // debug_sa("Setting var", &arg);

      eqpos = str_chr(arg.s, '=');
      arg.s[eqpos++] = '\0';
      v = &arg.s[eqpos];
      set_var(arg.s, v);

      ++optind;
      continue;
    }

    stralloc_replacec(&arg, PATHSEP_C == '/' ? '\\' : '/', PATHSEP_C);
    stralloc_nul(&arg);

#if WINDOWS_NATIVE && !MINGW
    if(str_rchrs(argv[optind], "*?", 2) < str_len(argv[optind]))
      strarray_glob(&args, arg.s);
    else
#endif
      strarray_push(&args, arg.s);
    ++optind;
  }

  /* No arguments given */
  if(strarray_size(&args) == 0) {

    buffer_putsflush(buffer_2, "ERROR: No arguments given\n\n");
    usage(argv[0]);
    ret = 1;
    goto exit;

  } else {
    target* rule;
    char** arg;
    target* all = get_rule("all");

    if(strlist_count(&workdir) && !stralloc_equals(&workdir.sa, ".")) {

      if((rule = get_rule_sa(&workdir.sa))) {
        stralloc_weak(&rule->recipe, &mkdir_command);

        add_path_sa(&all->prereq, &workdir.sa);
      }
    }

    hmap_init(1024, &sourcedirs);

    strarray_dump(buffer_2, &args);

    strarray_foreach(&args, arg) {

      if(!path_exists(*arg)) {
        buffer_putm_internal(buffer_2, "ERROR: Doesn't exist: ", *arg, newline, 0);
        buffer_flush(buffer_2);
        ret = 127;
        goto fail;
      }

      if(str_end(*arg, ".c"))
        add_source(*arg, &srcs);
      else
        get_sources(*arg, &srcs);
    }

    if(str_start(make, "g")) {
      target* compile = get_rule(".c.o");
      stralloc_weak(&compile->recipe, &compile_command);
    }
    if(((batch | shell) && stralloc_equals(&workdir.sa, ".")))
      batchmode = 1;

    populate_sourcedirs(&srcs, sourcedirs);

#ifdef DEBUG_OUTPUT
    dump_sourcedirs(buffer_2, sourcedirs);
#endif
    if(cmd_libs) {
      gen_lib_rules(rules, sourcedirs);

      deps_for_libs(rules);
    }

    if(cmd_bins) {
      cmd_bins = gen_link_rules(rules, &srcs);
    }

    if(cmd_bins == 0 || cmd_libs == 1) {
      TUPLE* t;
      hmap_foreach(rules, t) {
        target* tgt = hmap_data(t);

        if(stralloc_equal(&tgt->recipe, &lib_command))
          strlist_push(&all->prereq, t->key);
      }
    }

    gen_clean_rule(rules);

    {
      TUPLE* t;
      hmap_foreach(rules, t) {
        target* tgt = hmap_data(t);
        // print_target_deps(buffer_2, tgt);
      }
    }
    if(inst_bins || inst_libs)
      gen_install_rules(rules);

  fail:
    buffer_putm_internal(buffer_1, comment, " Generated by:", newline, comment, "  ", 0);
    buffer_putsa(buffer_1, &cmdline.sa);
    buffer_putsflush(buffer_1, newline);

    output_all_vars(buffer_1, vars);

    if(ninja) {
      output_build_rules(buffer_1, "cc", &compile_command);
      output_build_rules(buffer_1, "link", &link_command);
      output_build_rules(buffer_1, "lib", &lib_command);
      put_newline(buffer_1, 0);
    }

    if(batch || shell) {
      if(batch) {
        buffer_putm_internal(buffer_1, "CD %~dp0", newline, 0);
      } else {
        buffer_putm_internal(buffer_1, "cd \"$(dirname \"$0\")\"\n\n", 0);
      }

      output_script(buffer_1, NULL);
    } else
      output_all_rules(buffer_1, rules);

    //   hmap_dump(sourcedirs, buffer_1);

    hmap_destroy(&sourcedirs);
  }

exit:
  return ret;
}
