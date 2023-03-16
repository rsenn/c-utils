#include <assert.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include "lib/uint32.h"
#include "lib/byte.h"
#include "lib/stralloc.h"
#include "lib/buffer.h"
#include "lib/strlist.h"
#include "lib/path.h"
#include "lib/path_internal.h"
#include "genmakefile.h"
#include "lib/map.h"
#include "debug.h"
#include "lib/unix.h"
#include "lib/sig.h"
#include "lib/env.h"
#include "lib/glob.h"
#include "lib/dlist.h"
#include "lib/bool.h"
#include "src/genmakefile/is.h"
#include "src/genmakefile/path.h"
#include "src/genmakefile/rule.h"
#include "src/genmakefile/mplab.h"
#include "src/genmakefile/cmake.h"
#include "src/genmakefile/sources.h"
#include "src/genmakefile/sourcedir.h"
#include <string.h>

#if !WINDOWS_NATIVE
#include <unistd.h>
#else
#include <io.h>
#include <sys/stat.h>
#endif

#define YELLOW256 "\033[38;5;220m"
#define PURPLE256 "\033[38;5;90m"
#define BLUE256 "\033[38;5;27m"
#define GREEN256 "\033[38;5;70m"
#define YELLOW "\x1b[1;33m"
#define PINK256 "\x1b[38;5;201m"
#define RED "\x1b[1;31m"
#define NC "\x1b[0m"

#define REMOVE(sa, i, x, n) \
  size_t j = n; \
  i = stralloc_findb((sa), x, n); \
\
  if(i < (sa)->len) { \
    int q = i > 0 && (sa)->s[i - 1] == '"'; \
    if(q) { \
      i--; \
      j++; \
      if((sa)->s[i + j] == '"') \
        j++; \
    } \
    if((sa)->s[i + j] == ' ') \
      j++; \
    stralloc_replace((sa), i, j, "", 0); \
  }

extern buffer* unix_optbuf;
static const char tok_charset[] = {'_', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O',
                                   'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e',
                                   'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u',
                                   'v', 'w', 'x', 'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

void debug_int(const char* name, int i);
void debug_sa(const char* name, stralloc* sa);
void debug_sl(const char* name, const strlist* l, const char* sep);
void debug_str(const char* name, const char* s);

const char* const build_types[] = {"Release", "RelWithDebInfo", "MinSizeRel", "Debug"};
typedef void(linklib_fmt)(const char*, stralloc*);
int cmd_objs = 0, cmd_libs = 0, cmd_bins = 0;
static union commands commands;
static const char* libpfx = DEFAULT_LIBPFX;
static const char *make_begin_inline, *make_sep_inline, *make_end_inline;
static const char* comment = "#";
static const char* cross_compile = "";
static const char *srcdir_varname = "DISTDIR", *builddir_varname = "BUILDDIR";
static stralloc output_name;
static strlist vpath;
static char pathsep_make = DEFAULT_PATHSEP, pathsep_args = DEFAULT_PATHSEP;
static const char* quote_args = "";
static strlist build_as_lib;
strlist include_dirs = {0}, link_dirs = {0};
set_t link_libraries = {0, 0, 0, byte_hash};
set_t build_directories = {0, 0, 0, byte_hash};
// static strlist pptoks;
static const char* newline = "\n";
static int batch, shell, ninja;
static int batchmode;
static linklib_fmt* format_linklib_fn;
static int inst_bins, inst_libs;
static int cygming;
static strlist system_path;
const char *outfile = NULL, *infile = NULL, *project_name = NULL;
static strarray dirstack;
static strarray bins, progs;
exts_t exts = {DEFAULT_OBJEXT, DEFAULT_LIBEXT, DEFAULT_DSOEXT, DEFAULT_EXEEXT, DEFAULT_PPSEXT};
dirs_t dirs;
tools_t tools;
config_t cfg = {.mach = {0, 0}, .sys = {0, 0}, .chip = {0, 0, 0}, .build_type = 1, .lang = LANG_CXX};
tool_config_t tool_config = 0;
MAP_T targetdirs;

void
map_keys(const MAP_T* m, strlist* out) {
  MAP_PAIR_T t;
  MAP_FOREACH(*m, t) {
    const char* key = MAP_ITER_KEY(t);
    size_t len = MAP_ITER_KEY_LEN(t);
    if(len > 0 && key[len - 1] == '\0')
      --len;
    strlist_pushb(out, key, len);
  }
}

void
dump_map_keys(const MAP_T* m) {
  strlist out;
  strlist_init(&out, '\n');
  map_keys(m, &out);
  buffer_puts(buffer_2, "keys:\n  ");
  buffer_putsl(buffer_2, &out, "\n  ");
  buffer_putnlflush(buffer_2);
}

/*#if  (defined(DEBUG_OUTPUT) || defined(_DEBUG)) && !defined(NDEBUG)
#include "debug.h"
void
debug_target(const target* t) {
  buffer_putm_internal(buffer_2, "name: ", t->name, "\n", NULL);
  buffer_puts(buffer_2, "output: ");
  buffer_putset(buffer_2, &t->output, " ", 1);
  buffer_puts(buffer_2, "\nprereq: ");
  buffer_putset(buffer_2, &t->prereq, " ", 1);
  buffer_puts(buffer_2, "\nrecipe: ");
  buffer_putsa(buffer_2, &t->recipe);
  buffer_putsflush(buffer_2, debug_nl);
}

#else
#define debug_sa(x, y)
#define debug_sl(x, y, sep)
#define debug_str(x, y)
#define debug_int(x, y)
#endif*/

#ifndef _WIN32
#define _mkdir mkdir
#endif

int
mkdir_sa(const stralloc* dir, int mode) {
  stralloc sa;
  stralloc_init(&sa);
  stralloc_copy(&sa, dir);
  if(sa.len > 0 && stralloc_endb(&sa, &pathsep_make, 1))
    sa.len -= 1;
  stralloc_nul(&sa);
  return _mkdir(sa.s, mode);
}

int
mkdir_components(strlist* dir, int mode) {
  int ret = 0;
  size_t i, n = strlist_count(dir);
  if(n > 0) {
    for(i = 1; i <= n; ++i) {
      strlist r = strlist_range(dir, 0, i);

#ifdef DEBUG_OUTPUT_
      debug_sa("mkdir_components", &r.sa);
#endif

      if(r.sa.len) {
        if(mkdir_sa(&r.sa, mode) == -1) {
          if(errno == EEXIST) {
            errno = 0;
            continue;
          }
          ret = -1;
          break;
        }
      }
    }
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
      if(!str_start(tools.make, "nmake"))
        stralloc_subst(sa, args, str_len(args), "$^", "$|");
      else
        stralloc_copys(sa, args);
      stralloc_cats(sa, make_end_inline);
    } else {
      stralloc_cats(sa, args);
    }
  }
  if(str_start(tools.make, "nmake"))
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
  size_t i = 0;
  buffer_puts(b, "[\n");
  while(p < e) {
    buffer_puts(b, "  ");
    buffer_putulong(b, i);
    buffer_puts(b, ": ");
    buffer_puts(b, *p);
    buffer_putc(b, '\n');
    p++;
    i++;
  }
  buffer_putsflush(b, "]\n");
}

/**
 * @brief extract_build_type
 * @param s
 * @return
 */
int
extract_build_type(const stralloc* s) {
  size_t i;
  for(i = 0; i < sizeof(build_types) / sizeof(build_types[0]); ++i) {
    if(stralloc_contains(s, build_types[i]))
      return i;
  }
  return -1;
}

void
extract_tokens(const char* x, size_t n, set_t* tokens) {
  while(n) {
    size_t i;
    if(*x == '\r' || *x == '\n')
      break;
    if((i = scan_noncharsetnskip(x, tok_charset, n)) == n)
      break;
    x += i;
    n -= i;
    if(*x == '\r' || *x == '\n')
      break;
    i = scan_charsetnskip(x, tok_charset, n);
    if(i > 0 && !(i == 7 && byte_equal(x, 7, "defined")))
      if(!(*x >= '0' && *x <= '9'))
        if(set_add(tokens, x, i) == 1) {

#ifdef DEBUG_OUTPUT_
          debug_byte("added tok", x, i);
#endif
        }
    if(i == n)
      break;
    x += i;
    n -= i;
  }
}

/**
 * @brief extract_pptok  Extract
 * preprocessor tokens directives
 * @param x
 * @param n
 * @param includes
 * @param sys
 */
void
extract_pptok(const char* x, size_t n, set_t* tokens) {
  while(n) {
    size_t i;
    if((i = scan_charsetnskip(x, " \t\r\n", n)) == n)
      break;
    x += i;
    n -= i;
    if(*x == '#') {
      x += 1;
      n -= 1;
      if((i = scan_charsetnskip(x, " \t\r", n)) == n)
        break;
      x += i;
      n -= i;
      if((i = scan_noncharsetnskip(x, " \t\r\n<\"", n)) == n)
        break;
      if(!(i == 7 && byte_equal(x, 7, "include"))) {
        if((i >= 2 && byte_equal(x, 2, "if"))) {
          x += i;
          n -= i;
          {
            size_t linelen = byte_chrs(x, n, "\r\n", 2);
            size_t commentpos = byte_findb(x, n, "//", 2);
            while(linelen > 0 && linelen < n) {
              if(x[linelen - 1] == '\\') {
                if(x[linelen] == '\r' && x[linelen + 1] == '\n')
                  linelen++;
                if(linelen + 1 < n) {
                  linelen += 1;
                  linelen += byte_chrs(&x[linelen], n - linelen, "\r\n", 2);
                  continue;
                }
              }
              break;
            }
            if(commentpos < linelen)
              linelen = commentpos;
            /*          buffer_puts(buffer_2,
               "pptoks: ");
                      buffer_put(buffer_2,
               x, linelen);
                      buffer_putnlflush(buffer_2);*/
            extract_tokens(x, linelen, tokens);
          }
        }
      }
    }
    if((i = byte_chr(x, n, '\n')) >= n)
      break;
    x += i;
    n -= i;
  }
}

void
extract_vars(const char* x, size_t len, set_t* s) {
  size_t i;
  set_clear(s);
  for(i = 0; i < len; ++i) {
    const char* p = &x[i];
    if(i + 4 <= len && *p == '$' && p[1] == '(') {
      size_t vlen;
      i += 2;
      vlen = byte_chr(&x[i], len - i, ')');
      set_add(s, &x[i], vlen);
      i += vlen;
      continue;
    }
  }
}

/**
 * @brief extract_includes  Extract
 * #include directives
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
          strlist_pushb(includes, x, i);
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
 * @brief format_linklib_lib  Output
 * library name (+".lib")
 * @param libname
 * @param out
 */
void
format_linklib_lib(const char* libname, stralloc* out) {
  stralloc_cats(out, libpfx);
  stralloc_cats(out, libname);
  stralloc_cats(out, exts.lib);
}

/**
 * @brief format_linklib_switch  Output
 * library name (+ leading "-l")
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
size_t
skip_comment(const char* p, size_t len) {
  return byte_finds(p, len, "*/");
}

/**
 * @brief main_scan  Checks if the given
 * source file contains a main()
 * function
 * @param x
 * @param n
 * @return
 */
int
main_scan(const char* p, size_t n) {
  const char *end = p + n, *x = p;
  while(x + 5 < end) {
    size_t i;
    if(x[0] == '/') {
      if(x[1] == '*') {
        i = skip_comment(x, n);
        x += i;
        continue;
      } else if(x[1] == '/') {
        i = byte_chr(x, n, '\n');
        x += i + 1;
      }
    } else if(!isalpha(x[0]) && x[0] != '_') {
      if(byte_equal(&x[1], 4, "main")) {
        ssize_t i = 0;
        if(x + 5 >= end)
          return 0;
        i += 5;
        x += i;
        /*        if(i > 4 &&
           !isspace(*(x - 5))) continue;
        */
        i = scan_whitenskip(x, end - x);
        if(x + i >= end)
          break;
        x += i;
        if(x + 1 < end && *x == '(')
          return 1;
      }
    }
    x++;
  }
  return 0;
}

/**
 * @brief main_present  Checks for
 * main() routine in source file
 * @param filename  Path to source file
 * @return          1 when yes, 0 when
 * no, -1 on error
 */
int
main_present(const char* filename) {
  char* x;
  size_t n;

  if(str_equal(exts.src, ".asm") && is_source(filename))
    return 1;

  if((x = (char*)path_mmap_read(filename, &n, &dirs.this.sa, pathsep_make))) {
    int ret = main_scan(x, n);
    mmap_unmap(x, n);
    return ret;
  }
  return -1;
}

void
includes_cppflags() {
  const char* dir;
  stralloc arg;
  stralloc_init(&arg);
  strlist_foreach_s(&include_dirs, dir) {

    stralloc_zero(&arg);
    stralloc_cats(&arg, dir);
    // path_relative_to(dir, dirs.this.sa.s, &arg);

#ifdef DEBUG_OUTPUT_
    buffer_puts(buffer_2, "include_dir: ");
    buffer_putsa(buffer_2, &arg);
    buffer_putnlflush(buffer_2);
#endif

    stralloc_prepends(&arg, "-I");
    var_push_sa("CPPFLAGS", &arg);
  }
  stralloc_free(&arg);
}

/**
 * @brief includes_get
 * @param srcfile
 * @param includes
 * @param sys
 * @return
 */
int
includes_get(const char* srcfile, strlist* includes, int sys) {
  const char* x;
  size_t n;
  if((x = path_mmap_read(srcfile, &n, &dirs.this.sa, pathsep_make))) {
    extract_includes(x, n, includes, sys);
    mmap_unmap(x, n);
    return 1;
  }
  return 0;
}

void
includes_add_b(const char* dir, size_t len) {
  static stralloc abs;
  stralloc_zero(&abs);
  path_normalize_b(dir, len, &abs, &dirs.build.sa, &dirs.out.sa);
  if(strlist_push_unique_sa(&include_dirs, &abs)) {
#ifdef DEBUG_OUTPUT
    buffer_puts(buffer_2, "Added to include dirs: ");
    buffer_putsa(buffer_2, &abs);
    buffer_putnlflush(buffer_2);
#endif
  }
}

void
includes_add(const char* dir) {
  includes_add_b(dir, str_len(dir));
}

void
libdirs_add(const char* dir) {
  static stralloc abs;
  stralloc_zero(&abs);
  path_normalize(dir, &abs, &dirs.build.sa, &dirs.out.sa);
  if(strlist_push_unique_sa(&link_dirs, &abs)) {

#ifdef DEBUG_OUTPUT_
    buffer_puts(buffer_2, "Added to lib dirs: ");
    buffer_putsa(buffer_2, &abs);
    buffer_putnlflush(buffer_2);
#endif
  }
}

/**
 * @brief includes_to_libs  Include list
 * to library list
 * @param includes
 * @param libs
 */
void
includes_to_libs(const set_t* includes, strlist* libs) {
  const char* s;
  size_t n;
  stralloc sa, lib;
  set_iterator_t it;
  stralloc_init(&sa);
  stralloc_init(&lib);
  stralloc_zero(&libs->sa);
  set_foreach(includes, it, s, n) {
    target* rule;
    stralloc_zero(&sa);
    path_append(s, n, &sa);
    if(!(n > str_len(libpfx) && byte_equal(s, str_len(libpfx), libpfx)) && byte_chr(s, n, PATHSEP_C) == n) {
      path_concatb(libpfx, str_len(libpfx), sa.s, sa.len, &sa);
    }
    path_concatb(dirs.this.sa.s, dirs.this.sa.len, sa.s, sa.len, &sa);
    // debug_sa("include", &sa);
    stralloc_zero(&lib);
    stralloc_copys(&lib, path_basename(sa.s));
    if(stralloc_endb(&lib, exts.inc, 2))
      lib.len -= 2;
    stralloc_cats(&lib, exts.lib);
    // debug_sa("includes_to_libs", &lib);
    if((rule = rule_find_sa(&lib))) {

#ifdef DEBUG_OUTPUT_
      debug_str("lib", rule->name);
#endif

      strlist_push(libs, rule->name);
    }
  }
  stralloc_free(&lib);
  stralloc_free(&sa);
}

/**
 * @brief var_subst
 * @param in
 * @param out
 * @param pfx
 * @param sfx
 * @param tolower
 */
void
var_subst(const stralloc* in, stralloc* out, const char* pfx, const char* sfx, int tolower) {
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
 * Add a path to a strlist
 */
void
add_path_b(set_t* s, const char* path, size_t len) {
  if(set_has(s, path, len))
    return;
  set_insert(s, path, len);
}

/**
 * Add a path to a strlist
 */
void
add_path(set_t* s, const char* path) {
  add_path_b(s, path, str_len(path));
}

/**
 * @brief add_srcpath
 * @param list
 * @param path
 */
void
add_srcpath(set_t* s, const char* path) {
  static stralloc sa;
  if(srcdir.len && !stralloc_equals(&srcdir, ".")) {
    stralloc_zero(&sa);
    path_prefix_s(&srcdir, path, &sa, pathsep_make);
    set_addsa(s, &sa);
    stralloc_free(&sa);
  } else {
    set_adds(s, path);
  }
}

void
add_source(set_t* s, const char* path) {
  static stralloc sa;
  if(srcdir.len && !stralloc_equals(&srcdir, ".")) {
    stralloc_zero(&sa);
    path_prefix_s(&srcdir, path, &sa, pathsep_make);
    set_addsa(s, &sa);
  } else {
    set_adds(s, path);
  }
}

/**
 * @brief add_path_sa
 * @param list
 * @param path
 */
void
add_path_sa(set_t* s, stralloc* path) {
  add_path_b(s, path->s, path->len);
}

void
add_path_relativeb(set_t* s, stralloc* dir, const char* path, size_t pathlen) {
  stralloc sa;
  stralloc_init(&sa);
  path_concatb(dir->s, dir->len, path, pathlen, &sa);
  sa.len = path_collapse(sa.s, sa.len);
  add_path_sa(s, &sa);
  stralloc_free(&sa);
}

static int
count_b(strlist* list, int (*fn_b)(const char*, size_t)) {
  const char* x;
  size_t n;
  int ret = 0;
  strlist_foreach(list, x, n) {
    if(is_b(x, n, fn_b))
      ret++;
  }
  return ret;
}

/**
 * @brief push_lib  Add library spec to
 * variable
 * @param name
 * @param lib
 */
void
push_lib(const char* name, const char* lib) {
  stralloc sa;
  // var_t* var = var_list(name);
  stralloc_init(&sa);
  if(format_linklib_fn) {
    format_linklib_fn(lib, &sa);
    var_push_sa(name, &sa);
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
  var_push_sa("DEFS", &def);
  stralloc_copys(&lib64, lib);
  stralloc_cats(&lib64, "$(L64)");
  stralloc_nul(&lib64);
  push_lib("LIBS", lib64.s);
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
  var_push_sa("DEFS", &define);
}

/**
 * @}
 */
/**
 * @brief get_rules_by_cmd  Search rules
 * by command
 * @param cmd
 * @param deps
 */
void
get_rules_by_cmd(stralloc* cmd, strlist* deps) {
  MAP_PAIR_T t;
  MAP_FOREACH(rules, t) {
    target* rule = MAP_ITER_VALUE(t);
    if(rule->recipe.s == cmd->s || stralloc_equal(&rule->recipe, cmd)) {
      strlist_push(deps, rule->name);
    }
  }
}

/**
 * @brief dirname_alloc  Gets directory
 * name from a file path (allocated).
 * @param p
 * @return
 */
char*
dirname_alloc(const char* p) {
  size_t len = str_len(p);
  size_t pos = str_rchrs(p, PATHSEP_S_MIXED, sizeof(PATHSEP_S_MIXED) - 1);
  if(pos < len)
    return str_ndup(p, pos);
  return str_dup(".");
}

/*void
sourcedir_addsource(const char* source, strarray* srcs) {
  stralloc r, dir, tmp;
  strlist l;
  size_t n, dlen;
  const char *x, *s;
  set_iterator_t it;
  sourcedir* srcdir;
  sourcefile* file = sources_new(source, exts.bin, &progs, &bins);
  stralloc_init(&dir);
  stralloc_init(&tmp);
  strlist_init(&l, '\0');
  stralloc_init(&r);
  strlist_zero(&l);
  path_dirname(source, &dir);
  stralloc_nul(&dir);
  dlen = dir.len;
  source = path_clean_s(source, &dirs.this.sa);
  srcdir = sourcedir_getsa(&dir);
  slist_add(&srcdir->sources, &file->link);
  struct dnode* node = alloc(sizeof(struct dnode) + sizeof(sourcefile*));
  dlist_data(node, sourcefile*) = file;
  dlist_push(&sourcelist, node);
  ++srcdir->n_sources;
  if(!path_exists(source)) {

#ifdef DEBUG_OUTPUT
    buffer_puts(buffer_2, "Path doesn't exist: ");
    buffer_puts(buffer_2, source);
    buffer_putnlflush(buffer_2);
#endif

    return;

#if defined(__x86_64__) || defined(__i386__)
    __asm__("int3");
#endif
  }
  if((x = path_mmap_read(source, &n, &dirs.this.sa, pathsep_make)) != 0) {
    extract_includes(x, n, &l, 0);
    extract_pptok(x, n, &file->pptoks);
    mmap_unmap(x, n);
  }
  sources_addincludes(file, srcdir, &l, srcs, &exts.src, &dirs.this.sa, &dirs.out.sa);
  set_foreach_it(&file->pptoks, it) {
    x = set_iterator_value(&it, &n);
    if(n >= 5 && (byte_equal(x, 4, "USE_") || byte_equal(x, 5, "HAVE_")))
      set_add(&srcdir->pptoks, x, n);
  }
  stralloc_replacec(&l.sa, PATHSEP_C == '\\' ? '/' : '\\', PATHSEP_C);
  strlist_foreach(&l, s, n) {
    dir.len = dlen;
    stralloc_catc(&dir, PATHSEP_C);
    stralloc_catb(&dir, s, n);
    stralloc_nul(&dir);
    stralloc_copy(&r, &dir);
    path_canonical_sa(&r);
    set_addsa(&srcdir->includes, &r);
  }
  dir.len = dlen;
  stralloc_free(&r);
  strlist_free(&l);
  stralloc_free(&dir);
}

sourcedir*
sourcedir_find(const char* path) {
  sourcedir** ptr;
  if((ptr = MAP_GET(sourcedirs, path, str_len(path) + 1)))
    return *ptr;
  return 0;
}

sourcedir*
sourcedir_findsa(stralloc* path) {
  sourcedir** ptr;
  stralloc_nul(path);
  if((ptr = MAP_GET(sourcedirs, path->s, path->len + 1)))
    return *ptr;
  return 0;
}

sourcedir*
sourcedir_findb(const char* x, size_t n) {
  sourcedir* ret;
  stralloc p;
  stralloc_init(&p);
  stralloc_copyb(&p, x, n);
  ret = sourcedir_findsa(&p);
  stralloc_free(&p);
  return ret;
}

sourcedir*
sourcedir_getb(const char* x, size_t n) {
  sourcedir *s, **ptr;
  if(!(s = sourcedir_findb(x, n))) {
    sourcedir* newdir;
    newdir = alloc_zero(sizeof(sourcedir));
    set_init(&newdir->pptoks, 0);
    MAP_INSERT(sourcedirs, x, n + 1, &newdir, sizeof(newdir));
    if((ptr = (sourcedir**)MAP_GET(sourcedirs, x, n + 1)))
      s = *ptr;
  }
  return s;
}

sourcedir*
sourcedir_getsa(stralloc* path) {
  stralloc_nul(path);
  return sourcedir_getb(path->s, path->len);
}

void
sourcedir_populate(strarray* srcs) {
  MAP_PAIR_T t;
  strlist d;
  const char* x;
  size_t n;
  strlist_init(&d, '\0');

  MAP_FOREACH(sourcedirs, t) {
    sourcedir* dir = *(sourcedir**)MAP_ITER_VALUE(t);
    sourcefile* file;

#ifdef DEBUG_OUTPUT_
    const char* name = MAP_ITER_KEY(t);
    buffer_puts(buffer_2, "sourcedir: ");
    buffer_puts(buffer_2, name);
    buffer_putnlflush(buffer_2);
#endif

    strlist_zero(&d);
    slist_foreach(dir->sources, file) {
      sources_deps(file, &d);
      strlist_foreach(&d, x, n) { set_add(&file->deps, x, n); }
    }
  }
  MAP_FOREACH(sourcedirs, t) {
    sourcedir* dir = *(sourcedir**)MAP_ITER_VALUE(t);
    strlist_zero(&d);
    sourcedir_deps(dir, &d);
    strlist_foreach(&d, x, n) { set_add(&dir->deps, x, n); }
  }
}

void
sourcedir_dump_all(buffer* b) {
  MAP_PAIR_T t;
  MAP_FOREACH(sourcedirs, t) {
    sourcedir* srcdir = *(sourcedir**)MAP_ITER_VALUE(t);
    sourcefile* pfile;
    buffer_puts(b, " '");
    buffer_put(b, MAP_ITER_KEY(t), str_len(MAP_ITER_KEY(t)));
    buffer_puts(b, "' (");
    buffer_putulong(b, srcdir->n_sources);
    buffer_puts(b, "): [");
    slist_foreach(srcdir->sources, pfile) {
      buffer_putspace(b);
      buffer_puts(b, pfile->name);
    }
    buffer_puts(b, " ]");
    buffer_putnlflush(b);
  }
}

void
sourcedir_dep_recursive(sourcedir* srcdir, strlist* out, uint32 serial, sourcedir* parent) {
  const char* s;
  size_t n;
  set_iterator_t it;
  sourcedir* sdir;
  if(srcdir->serial == serial)
    return;
  set_foreach(&srcdir->deps, it, s, n) {
    if(!strlist_containsb(out, s, n)) {
      if((sdir = sourcedir_findb(s, n)) && sdir != srcdir) {
        if(sdir->serial == serial)
          continue;
        if(sdir == parent)
          continue;
        srcdir->serial = serial;
        sourcedir_dep_recursive(sdir, out, serial, srcdir);
        strlist_pushb(out, s, n);
      }
    }
  }
}

void
sourcedir_deps(sourcedir* srcdir, strlist* out) {
  uint32 serial = uint32_random();
  return sourcedir_dep_recursive(srcdir, out, serial, 0);
}

void
sourcedir_deps_s(const char* srcdir, strlist* out) {
  sourcedir* sdir = sourcedir_getb(srcdir, str_len(srcdir) + 1);
  assert(sdir);
  return sourcedir_deps(sdir, out);
}

void
sourcedir_deps_b(const char* sdir, size_t sdirlen, strlist* out) {
  stralloc sa;
  stralloc_init(&sa);
  stralloc_copyb(&sa, sdir, sdirlen);
  return sourcedir_deps_s(sa.s, out);
}

void
sourcedir_printdeps(sourcedir* srcdir, buffer* b, int depth) {
  const char* s;
  size_t n;
  set_iterator_t it;
  sourcedir* sdir;
  strlist deps;
  strlist_init(&deps, '\0');
  set_foreach(&srcdir->deps, it, s, n) {
    if(strlist_pushb_unique(&deps, s, n)) {
      buffer_putnspace(buffer_2, depth * 2);
      buffer_put(buffer_2, s, n);
      buffer_putnlflush(buffer_2);
      if((sdir = sourcedir_findb(s, n)))
        sourcedir_printdeps(sdir, b, depth + 1);
    }
  }
  strlist_free(&deps);
}
*/
void
builddir_enter(const char* x, size_t len) {
  stralloc dir;
  stralloc_init(&dir);
  strarray_emplace_sa(&dirstack, &dirs.build.sa);
  stralloc_copyb(&dirs.build.sa, x, len);

  path_relative_b(dirs.build.sa.s, dirs.build.sa.len, &dir);
  set_addsa(&build_directories, &dir);

#ifdef DEBUG_OUTPUT
  buffer_puts(buffer_2, "Entering [");
  buffer_putlong(buffer_2, strarray_size(&dirstack));
  buffer_puts(buffer_2, "] '");
  buffer_putsa(buffer_2, &dir);
  buffer_puts(buffer_2, "'");
  buffer_putnlflush(buffer_2);
#endif

  stralloc_free(&dir);
}

void
builddir_leave(const char* x, size_t len) {
#ifdef DEBUG_OUTPUT
  buffer_puts(buffer_2, "Leaving [");
  buffer_putlong(buffer_2, strarray_size(&dirstack));
  buffer_puts(buffer_2, "] '");
  buffer_put(buffer_2, x, len);
  buffer_puts(buffer_2, "'");
  buffer_putnlflush(buffer_2);
#endif

  stralloc_free(&dirs.build.sa);

  dirs.build.sa.s = strarray_pop(&dirstack);
  dirs.build.sa.len = str_len(dirs.build.sa.s);
}

/**
 *
 * @brief deps_indirect
 * @param l
 * @param names
 */
void
deps_indirect(set_t* s, const strlist* names) {
  size_t n;
  const char* x;
  target* t;
  strlist hier;
  strlist_init(&hier, '\0');
  strlist_foreach(names, x, n) {
    if((t = rule_find_b(x, n))) {
      strlist_pushb(&hier, x, n);
      rule_dep_list_recursive(t, s, -1, &hier);
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
deps_direct(set_t* s, const target* t) {
  target** ptr;
  array_foreach_t(&t->deps, ptr) {
    if(*ptr) {
      set_adds(s, (*ptr)->name);
    }
  }
}

/**
 * @brief print_rule_deps_r
 * @param b
 * @param t
 * @param deplist
 * @param hierlist
 * @param depth
 */
void
print_rule_deps_r(buffer* b, target* t, set_t* deplist, strlist* hierlist, int depth) {
  target** ptr;
  size_t l = hierlist->sa.len;
  /*if(strlist_contains(&hierlist,
    t->name)) return;
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
      // buffer_putsa(b,   &hierlist->sa);
      buffer_putnspace(b, depth * 2);
      buffer_puts(b, str_basename(t->name));
      buffer_puts(b, " -> ");
      buffer_puts(b, str_basename(name));
      put_newline(b, 1);
      if(set_adds(deplist, name))
        print_rule_deps_r(b, (*ptr), deplist, hierlist, depth + 1);
    }
  }
  hierlist->sa.len = l;
  // strlist_trunc(hierlist, depth);
}

/**
 * @brief print_rule_deps  Prints
 * dependency tree for a target
 * @param b                  Output
 * buffer
 * @param t                  Target
 */
void
print_rule_deps(buffer* b, target* t) {
  set_t deplist;
  strlist hierlist;
  set_init(&deplist, 0);
  strlist_init(&hierlist, 0);
  set_adds(&deplist, t->name);
  buffer_putm_internal(b, "# Dependencies for '", t->name, "':", NULL);
  buffer_putnlflush(b);
  print_rule_deps_r(b, t, &deplist, &hierlist, 0);
  set_free(&deplist);
  strlist_free(&hierlist);
}

/**
 * @brief remove_indirect_deps_recursive
 * Removes all indirect dependencies
 * @param top Toplevel dependencies
 * @param a Dependency layer array
 * @param depth Recursion depth
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
 * @brief deps_for_libs
 * @param rules
 */
void
deps_for_libs() {
  MAP_PAIR_T t;
  strlist deps;
  set_t indir;
  stralloc sa;
  strlist_init(&deps, '\0');
  set_init(&indir, 0);
  stralloc_init(&sa);
  MAP_FOREACH(sourcedirs, t) {
    sourcedir* srcdir = *(sourcedir**)MAP_ITER_VALUE(t);
    target* lib;
    stralloc_zero(&sa);
    path_prefix_s(&dirs.work.sa, str_basename(MAP_ITER_KEY(t)), &sa, pathsep_make);
    stralloc_cats(&sa, exts.lib);
    if((lib = rule_find_sa(&sa))) {
      strlist libs;
      strlist_init(&libs, ' ');
      includes_to_libs(&srcdir->includes, &libs);
      // debug_str("library", lib->name);
      // debug_sl("includes", &srcdir->includes);
      strlist_removes(&libs, lib->name);
      // debug_sl("deps", &libs);
      set_clear(&indir);
      deps_indirect(&indir, &libs);
      // debug_set("indir", &indir, "
      //");
      /*  strlist_sub(&indir, &libs);
        strlist_sub(&libs, &indir);*/

#ifdef DEBUG_OUTPUT_
      buffer_putm_internal(buffer_2, "Deps for library '", lib->name, "': ", NULL);
      buffer_putsa(buffer_2, &libs.sa);
      buffer_putnlflush(buffer_2);
#endif

      target_ptrs(&libs, &lib->deps);
      // print_rule_deps(buffer_2, lib);
      strlist_free(&libs);
    }
  }
  stralloc_free(&sa);
}

/**
 * @brief target_ptrs  Given a list of
 * target names, outputs an array of
 * pointers to those targets.
 * @param targets      Target names
 * @param out          Output array
 */
void
target_ptrs(const strlist* targets, array* out) {
  const char* x;
  size_t n;
  strlist_foreach(targets, x, n) {
    target* rule;
    if((rule = rule_find_b(x, n))) {
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

target*
gen_single_rule(stralloc* output, stralloc* cmd) {
  target* rule;
  if((rule = rule_get_sa(output))) {
    set_init(&rule->prereq, 0);
    if(cmd && !(rule->recipe.len && stralloc_equal(&rule->recipe, cmd))) {
      if(rule->recipe.len)
        stralloc_cats(&rule->recipe, "\n");
      stralloc_cat(&rule->recipe, cmd);
    }
  }
  return rule;
}

/**
 * @brief gen_clean_rule  Generate clean
 * rule which removes all target outputs
 * @param rules
 */
void
gen_clean_rule() {
  target* clean = rule_get("clean");
  static stralloc output;
  MAP_PAIR_T t;
  char* arg;
  size_t cmdoffs, lineoffs = 0;
  stralloc fn;
  strlist delete_args;
  stralloc_init(&fn);
  strlist_init(&delete_args, '\0');

  if(commands.delete.len == 0)
    stralloc_copys(&commands.delete, cfg.sys.type == NTOS ? "DEL /F /Q" : "$(RM)");

  cmdoffs = commands.delete.len;
  MAP_FOREACH(rules, t) {
    target* rule = MAP_ITER_VALUE(t);
    const char* name = MAP_ITER_KEY(t);

    if(rule->phony)
      continue;

    stralloc_zero(&output);
    /* Ignore the dirs.build rule */
    /*  if(stralloc_equals(&dirs.work.sa, MAP_ITER_KEY(t)))
        continue;*/

    if(name[str_chr(name, '%')]) {
      // strlist_nul(&rule->output);
      stralloc_catset(&delete_args.sa, &rule->output, " ");
      continue;
    }
    /* If the rule has prerequisites  and a recipe, it must be a producing rule */
    if(set_size(&rule->prereq) && rule->recipe.s) {
      size_t bpos;
      if((MAP_ITER_KEY(t))[(bpos = str_rchr(MAP_ITER_KEY(t), '{'))]) {
        size_t epos = str_rchr(&(MAP_ITER_KEY(t))[bpos + 1], '}');
        stralloc_zero(&fn);
        stralloc_catb(&fn, &(MAP_ITER_KEY(t)[bpos + 1]), epos);
        stralloc_catc(&fn, pathsep_make);
        stralloc_cats(&fn, "*");
        stralloc_catb(&fn,
                      &(MAP_ITER_KEY(t)[bpos + 1 + epos + 1]),
                      str_chr(&(MAP_ITER_KEY(t)[bpos + 1 + epos + 1]), ':'));
        stralloc_nul(&fn);
        arg = fn.s;
      } else {
        stralloc_copys(&fn, name);
        /* If possible, transform file name into a wildcard pattern */
        arg = path_wildcard(&fn, "*");
      }
      /* Add to deletion list */
      strlist_push_unique(&delete_args, arg);
    }
  }
  strlist_nul(&delete_args);
  strlist_foreach_s(&delete_args, arg) {
    if(commands.delete.len - lineoffs + str_len(arg) >= MAX_CMD_LEN) {
      stralloc_readyplus(&commands.delete, cmdoffs + 3);
      stralloc_catm_internal(&commands.delete, newline, "\t", NULL);
      stralloc_catb(&commands.delete, commands.delete.s, cmdoffs);
      lineoffs = commands.delete.len;
    }
    stralloc_catc(&commands.delete, ' ');
    stralloc_cats(&commands.delete, arg);
    if(arg[str_chr(arg, '*')])
      lineoffs = -MAX_CMD_LEN;
  }
  stralloc_weak(&clean->recipe, &commands.delete);
}

int
filter_pptoks(const void* x, size_t n) {
  return byte_equal(x, n > 4 ? 4 : n, "USE_");
}

target*
gen_mkdir_rule(stralloc* dir) {
  target* rule = 0;
  if(stralloc_length(dir) && !stralloc_equals(dir, ".")) {
    if((rule = rule_get_sa(dir))) {
      stralloc_weak(&rule->recipe, &commands.mkdir);
    }
  }
  return rule;
}

/**
 * @brief gen_srcdir_compile_rules
 * Generate compile rules for every
 * source file given
 * @param rules                     All
 * rules
 * @param srcdir source dir structure
 * @param dir source dir path
 * @return
 */
target*
gen_srcdir_compile_rules(sourcedir* sdir, const char* dir) {
  sourcefile* src;
  target* rule = 0;
  MAP_PAIR_T t;
  stralloc target, srcs, obj;
  size_t len, tlen;
  strlist pptoks;
  const char* tok;
  set_iterator_t it;
  stralloc defines;
  stralloc_init(&target);
  path_output("%", &target, exts.obj, &dirs.build.sa, pathsep_args);
  stralloc_cats(&target, ": ");
  stralloc_init(&srcs);
  tlen = target.len;
  stralloc_init(&obj);
  strlist_init(&pptoks, '\0');
  set_foreach(&sdir->pptoks, it, tok, len) {
    if(filter_pptoks(tok, len))
      strlist_pushb(&pptoks, tok, len);
  }
  buffer_puts(buffer_2, "pptoks: ");
  strlist_dump(buffer_2, &pptoks);
  strlist_free(&pptoks);
  // strlist_filter(&sdir->pptoks, &filter_pptoks, &pptoks);
  stralloc_init(&defines);
  if(set_size(&sdir->pptoks) > 0) {
    stralloc_copys(&defines, "DEFS +=");
    set_foreach(&sdir->pptoks, it, tok, len) {
      if(filter_pptoks(tok, len)) {
        stralloc_cats(&defines, " -D");
        stralloc_catb(&defines, tok, len);
        stralloc_cats(&defines, "=1");
      }
    }
  }
  slist_foreach(sdir->sources, src) {
    const char* ext = 0;
    /*
      if(!src->name)
        continue;*/

#ifdef DEBUG_OUTPUT
    debug_str("Adding source", src->name);
#endif

    if(!is_source(src->name) /*&& !is_include(src->name)*/)
      continue;
    // s = str_basename(src->name);
    target.len = tlen;
    stralloc_zero(&srcs);
    path_prefix_s(&srcdir, src->name, &srcs, pathsep_make);
    path_wildcard(&srcs, "%");
    stralloc_replacec(&srcs, pathsep_make == '/' ? '\\' : '/', pathsep_make);
    stralloc_zero(&obj);
    path_output(src->name, &obj, exts.obj, &dirs.build.sa, pathsep_args);
    if(str_start(tools.make, "g") || ((shell | batch) && batchmode)) {
      stralloc_cat(&target, &srcs);
    } else if(batchmode) {
      stralloc_zero(&target);
      stralloc_catm_internal(&target, "{", dir, "}", ext, "{", dirs.work.sa.s, "}", exts.obj, ":", NULL);
    } else {
    }
    // stralloc_replacec(&target, pathsep_make == '/' ? '\\' : '/', pathsep_make);
    if(!rule)
      rule = rule_get_sa(&target);
    if(rule) {
      stralloc_copy(&rule->vars.sa, &defines);
      if(rule->recipe.s == 0) {
        set_clear(&rule->output);
        set_clear(&rule->prereq);
      }
      set_addsa(&rule->output, &obj);
      add_srcpath(&rule->prereq, srcs.s);
      if(rule->recipe.s)
        continue;
      if((shell | batch) == 0 && batchmode) {
        stralloc_init(&rule->recipe);
        stralloc_copy(&rule->recipe, &commands.compile);
        stralloc_replaces(&rule->recipe, "-Fo", "-Fd");
        stralloc_replaces(&rule->recipe, "$@", dirs.work.sa.s);
        continue;
      }
      {
        size_t p, e;
        char* x;
        if(dirs.work.sa.len == 0 || stralloc_equals(&dirs.work.sa, ".")) {
          stralloc_copy(&rule->recipe, &commands.compile);
          x = stralloc_begin(&rule->recipe);
          p = e = stralloc_finds(&rule->recipe, "$@");
          while(p > 0 && !((x[p + 1] == '/' || x[p + 1] == '-') && x[p] == ' '))
            p--;
          e += 2;
          if(x[e] == '"')
            e++;
          stralloc_remove(&rule->recipe, p, e - p);
        } else {
          stralloc_weak(&rule->recipe, &commands.compile);
        }
        /* if(stralloc_length(&rule->recipe) == 0)
           stralloc_copy(&rule->recipe, &commands.compile);*/
      }
    }
  }
  stralloc_free(&obj);
  stralloc_free(&target);
  stralloc_free(&defines);

  return rule;
}

/**
 * @brief gen_simple_compile_rules  compile rules for every  source file in srcdir
 * @param rules                     All  rules
 * @param srcdir source dir structure
 * @param dir source dir path
 * @return
 */
target*
gen_simple_compile_rules(sourcedir* srcdir, const char* dir, const char* fromext, const char* toext, stralloc* cmd) {
  sourcefile* src;
  stralloc ppsrc, obj;
  stralloc_init(&ppsrc);
  stralloc_init(&obj);

#ifdef DEBUG_OUTPUT_
  buffer_putm_internal(buffer_2, "gen_simple_compile_rules '", dir, "' ", fromext, " ", toext, NULL);
  buffer_putnlflush(buffer_2);
#endif

  slist_foreach(srcdir->sources, src) {
    target* rule;
    const char* base;
    const char* srcname = src->name;
    if(srcname == 0)
      continue;
    base = str_basename(srcname);
    if(!str_equal(base + str_rchr(base, '.'), fromext))
      continue;
    if(tools.preproc) {
      stralloc_zero(&obj);
      path_output(base, &obj, ".pp.c", &dirs.build.sa, pathsep_args);
      // stralloc_inserts(&ppsrc,  ".pp", byte_rchr(ppsrc.s,     ppsrc.len, '.'));
      if((rule = rule_get_sa(&obj))) {
        add_source(&rule->prereq, src->name);
        stralloc_copy(&rule->recipe, &commands.preprocess);
        ppsrc = obj;
        srcname = ppsrc.s;
      }
    }
    stralloc_init(&obj);
    path_output(base, &obj, toext, &dirs.build.sa, pathsep_args);
    /*    debug_str("base", base);
        debug_sa("obj", &obj);*/
    if((rule = rule_get_sa(&obj))) {
      add_source(&rule->prereq, srcname);
      if(rule->recipe.s == NULL) {
        stralloc_weak(&rule->recipe, cmd);
        array_catb(&srcdir->rules, &rule, sizeof(target*));
      }
    }
  }
  stralloc_free(&obj);
  stralloc_free(&ppsrc);
  return 0;
}

/**
 * @brief gen_srcdir_lib_rule  Generate  lib rule for source dir
 * @param rules                     All rules
 * @param srcdir source dir structure
 * @param dir source dir path
 * @return
 */
target*
gen_srcdir_lib_rule(sourcedir* srcdir, const char* name) {
  target *dep = 0, *rule;
  stralloc sa;
  stralloc_init(&sa);
  path_prefix_s(&dirs.work.sa, name, &sa, pathsep_make);
  // stralloc_copys(&sa, name);
  stralloc_cats(&sa, exts.lib);

#ifdef DEBUG_OUTPUT_
  debug_str("gen_srcdir_lib_rule", name);
  debug_sa("gen_srcdir_lib_rule", &sa);
#endif

  if((str_start(tools.make, "g") || batchmode) && cfg.mach.arch != PIC) {
    buffer_putm_internal(buffer_2, "gen_srcdir_compile_rules: ", name, NULL);
    buffer_flush(buffer_2);
    dep = gen_srcdir_compile_rules(srcdir, name);
  } else {
    /* if(0 && tools.preproc) {gen_simple_compile_rules(rules, srcdir, name, exts.src, exts.pps, &commands.preprocess);
     * dep = gen_simple_compile_rules(rules, srcdir, name, exts.pps, exts.obj, &commands.compile); } else {*/
    dep = gen_simple_compile_rules(srcdir, name, exts.src, exts.obj, &commands.compile);
    //}
  }
  if((rule = rule_get_sa(&sa))) {
    sourcefile* pfile;
    set_init(&rule->prereq, 0);
    if(dep) {
      set_cat(&rule->prereq, &dep->output);
      array_catb(&rule->objs, &dep, sizeof(target*));
    } else {
      slist_foreach(srcdir->sources, pfile) {
        if(pfile->name == NULL || !is_source(pfile->name))
          continue;
        stralloc_zero(&sa);
        if(vpath.sa.len)
          path_extension(pfile->name, &sa, exts.obj);
        else
          path_output(pfile->name, &sa, exts.obj, &dirs.build.sa, pathsep_args);
        set_addsa(&rule->prereq, &sa);
      }
    }
    stralloc_weak(&rule->recipe, &commands.lib);
  }
  stralloc_free(&sa);
  return rule;
}

/**
 * @brief gen_srcdir_rule
 * @param rules                     All
 * rules
 * @param srcdir source dir structure
 * @param name source dir name
 */
void
gen_srcdir_rule(sourcedir* sdir, const char* name) {
  sourcefile* src;
  target* rule;
  stralloc mask;
  stralloc_init(&mask);
  slist_foreach(sdir->sources, src) {
    const char* s;

#ifdef DEBUG_OUTPUT_
    debug_str("sourcefile", src->name);
#endif

    if(!is_source(src->name))
      continue;
    s = str_ndup(src->name, str_rchr(src->name, '.'));
    stralloc_zero(&mask);
    path_prefix_s(&dirs.work.sa, str_basename(s), &mask, pathsep_make);
    stralloc_cats(&mask, exts.obj);
    if(batchmode || str_start(tools.make, "g")) {
      stralloc_cats(&mask, ": ");
      stralloc_cats(&mask, src->name);
      path_wildcard(&mask, "%");
    }
    // debug_sa("mask", &mask);
    if((rule = rule_get_sa(&mask))) {
      set_adds(&rule->prereq, src->name);
      if(rule->recipe.s == 0) {
        stralloc_weak(&rule->recipe, &commands.compile);
      }
    }
    free((void*)s);
  }
  stralloc_free(&mask);
}

/**
 * @brief gen_lib_rules  Generate
 * library rules for every source dir
 * @param rules
 * @param srcdirs
 */
void
gen_lib_rules() {
  MAP_PAIR_T t;
  target* all;
  stralloc inc, abspath;
  stralloc_init(&inc);
  stralloc_init(&abspath);
  all = rule_get("all");
  MAP_FOREACH(sourcedirs, t) {
    target* rule;
    sourcedir* srcdir = *(sourcedir**)MAP_ITER_VALUE(t);
    const char* base = path_basename(MAP_ITER_KEY(t));
    if(str_equal(base, ".")) {
      stralloc_zero(&abspath);
      path_absolute(MAP_ITER_KEY(t), &abspath);
      stralloc_nul(&abspath);
      base = path_basename(MAP_ITER_KEY(t));
    }

#ifdef DEBUG_OUTPUT_
    debug_str("srcdir", MAP_ITER_KEY(t));
    debug_str("base", base);
#endif

    if(strlist_contains(&build_as_lib, base) /* || (str_equal(base,
                                                "lib") &&
                                                cfg.mach.arch !=
                                                PIC)*/
       || base[0] == '.' || base[0] == '\0')
      continue;
    // gen_srcdir_rule(rules, srcdir, base);
    rule = gen_srcdir_lib_rule(srcdir, base);
    set_adds(&link_libraries, rule->name);
    set_adds(&all->prereq, rule->name);
    array_catb(&srcdir->rules, &rule, sizeof(target*));
  }
  stralloc_free(&inc);
  stralloc_free(&abspath);
}

target*
gen_program_rule(const char* filename) {
  target *preprocess = 0, *compile = 0, *rule = 0;
  const char *x, *link_lib;
  // char** p;
  set_t incs = SET(), deps = SET();
  strlist libs;
  stralloc dir, bin;
  stralloc ppsrc, obj;
  strlist_init(&libs, ' ');
  stralloc_init(&ppsrc);
  stralloc_init(&obj);
  stralloc_init(&dir);
  stralloc_init(&bin);

#ifdef DEBUG_OUTPUT_
  debug_str("gen_program_rule", filename);
#endif

  path_dirname(filename, &dir);
  sourcedir* srcdir = sourcedir_getsa(&dir);
  // gen_compile_rules(srcdir, dir.s);
  if(tools.preproc) {
    path_output(filename, &ppsrc, exts.pps, &dirs.build.sa, pathsep_args);
  }
  path_output(filename, &obj, exts.obj, &dirs.build.sa, pathsep_args);
  if(tools.preproc && (preprocess = rule_get_sa(&ppsrc))) {
    add_source(&preprocess->prereq, filename);
    stralloc_weak(&preprocess->recipe, &commands.preprocess);
  }
  if((compile = rule_get_sa(&obj))) {
    add_source(&compile->prereq, filename);
    stralloc_weak(&compile->recipe, &commands.compile);
    stralloc_zero(&compile->recipe);
    buffer_putm_internal(
        buffer_2, "\033[38;5;82mcompile rule" NC " '", compile->name, "' recipe '", compile->recipe.s, "'", NULL);
    buffer_putnlflush(buffer_2);
  }
  stralloc_zero(&bin);
  if(!cmd_libs) {
    stralloc outname;
    stralloc_init(&outname);
    stralloc_cats(&outname, path_basename(filename));
    if(stralloc_endb(&outname, exts.src, 2))
      outname.len -= 2;
    stralloc_nul(&outname);
    path_output(outname.s, &bin, exts.bin, &dirs.build.sa, pathsep_args);
  } else {
    path_extension(obj.s, &bin, exts.bin);
  }
  target* all = rule_get("all");
  add_path_b(&all->prereq, bin.s, bin.len);
  if((rule = rule_get_sa(&bin))) {
    size_t n;
    sourcefile* pfile;
    set_iterator_t it;
    add_path_sa(&rule->prereq, &obj);
    if(1 || cmd_libs) {
      slist_foreach(srcdir->sources, pfile) {
        if(!pfile->has_main && !is_include(pfile->name)) {
          stralloc_zero(&obj);
          path_output(pfile->name, &obj, exts.obj, &dirs.build.sa, pathsep_args);
          add_path_sa(&rule->prereq, &obj);
        }
      }
    } else {
      slink* source2;
      dlist_foreach_down(&sourcelist, source2) {
        sourcefile* sfile = dlist_data(source2, sourcefile*);
        char* filename = (char*)sfile->name;
        stralloc_zero(&dir);
        path_dirname(filename, &dir);
        if(str_end(filename, exts.inc))
          continue;
        strlist_push_unique_sa(&vpath, &dir);
        stralloc_zero(&obj);
        path_output(filename, &obj, exts.obj, &dirs.build.sa, pathsep_args);
        add_path_sa(&rule->prereq, &obj);
      }
    }
    stralloc_weak(&rule->recipe, &commands.link);
    stralloc_nul(&rule->recipe);
    buffer_putm_internal(buffer_2, "" BLUE256 "link rule" NC " '", rule->name, "' recipe '", rule->recipe.s, "'", NULL);
    buffer_putnlflush(buffer_2);
    includes_to_libs(&incs, &libs);
    target_ptrs(&libs, &rule->deps);
    set_clear(&deps);
    rule_dep_list(rule, &deps);
    set_foreach(&link_libraries, it, link_lib, n) {
      const char* s;
      size_t len;
      target* lib = rule_find_b(link_lib, n);
      set_foreach(&lib->prereq, it, s, len) { set_add(&deps, s, len); }
      add_path(&all->prereq, lib->name);
    }
    if(set_size(&deps)) {
      set_foreach(&deps, it, x, n) { set_add(&rule->prereq, x, n); }
    }
  }
  stralloc_free(&bin);
  stralloc_free(&obj);
  stralloc_free(&dir);
  return rule;
}

/**
 * @brief gen_link_rules  Generate
 * compile rules for every source file
 * with a main()
 * @param rules
 * @param sources
 * @return
 */
int
gen_link_rules(/*strarray* sources*/) {
  int num_main = 0, count = 0;
  target* link = 0;
  set_t incs, deps;
  strlist libs, indir;
  struct dnode* node;
  set_init(&incs, 0);
  set_init(&deps, 0);
  strlist_init(&libs, ' ');
  strlist_init(&indir, ' ');
  dlist_foreach_down(&sourcelist, node) {
    sourcefile* file = dlist_data(node, sourcefile*);
    char* filename = (char*)file->name;

#ifdef DEBUG_OUTPUT_
    buffer_putm_internal(buffer_2, "GEN_LINK_RULES file = ", filename, NULL);
    buffer_putnlflush(buffer_2);
#endif

    set_clear(&incs);
    strlist_zero(&libs);
    set_clear(&deps);
    strlist_zero(&indir);
    if(is_source(filename) && main_present(filename)) {
      target* rule = gen_program_rule(filename);
      if(!link && rule)
        link = rule;
      strarray_push_unique(&progs, filename);
      num_main++;
    }
    count++;
  }
  if(num_main <= 1 && link && output_name.len) {
    stralloc oldname;
    stralloc_init(&oldname);
    stralloc_catset(&oldname, &link->output, " ");
    if(!stralloc_endb(&output_name, exts.bin, str_len(exts.bin)))
      stralloc_cats(&output_name, exts.bin);
    stralloc_nul(&output_name);
    rule_rename(link, output_name.s, pathsep_make);
    // if(set_has_sa(&all->prereq, &oldname)) {
    //}
    /*  if((pos =
      stralloc_find(&all->prereq.sa,
      &oldname)) < all->prereq.sa.len) {
        stralloc_replace(&all->prereq.sa,
      pos, oldname.len,
      link->output.sa.s,
      link->output.sa.len);
      }*/
  }
  set_free(&incs);
  strlist_free(&libs);
  set_free(&deps);
  strlist_free(&indir);
  return count;
}

/**
 * @brief gen_install_rules
 * @param rules
 * @return
 */
target*
gen_install_rules() {
  MAP_PAIR_T t;
  target* inst = NULL;
  const char* v = 0;
  MAP_FOREACH(rules, t) {
    target* rule = MAP_ITER_VALUE(t);
    int do_lib, do_bin;
    do_lib = (inst_libs && (str_end(MAP_ITER_KEY(t), ".lib") || str_end(MAP_ITER_KEY(t), ".a") ||
                            MAP_ITER_KEY(t)[str_find(MAP_ITER_KEY(t), ".so")] || rule->recipe.s == commands.lib.s));
    do_bin = (inst_bins && (str_end(MAP_ITER_KEY(t), ".dll") || str_end(MAP_ITER_KEY(t), ".exe") ||
                            rule->recipe.s == commands.link.s));
    if(!(do_lib || do_bin))
      continue;
    if(!inst) {
      inst = rule_get("install");
      // inst->recipe =
      // malloc(sizeof(stralloc));
      stralloc_init(&inst->recipe);
      set_adds(&inst->prereq, "all");
    }
    if(!var_isset("prefix")) {
      var_set("prefix", "/usr");
      stralloc_catm_internal(&inst->recipe, newline, "\t$(INSTALL_DIR) $(DESTDIR)$(prefix)", NULL);
      if(!v) {
        v = var_set("INSTALL", "install")->value.sa.s;
        var_set("INSTALL_DIR", str_start(v, "install") ? "$(INSTALL) -d" : "mkdir");
        if(do_lib)
          var_set("INSTALL_DATA", str_start(v, "install") ? "$(INSTALL) -m 644" : "$(INSTALL)");
      }
    }
    if(do_bin) {
      if(!var_isset("bindir")) {
        var_set("bindir", "$(prefix)/bin");
        stralloc_catm_internal(&inst->recipe, newline, "\t$(INSTALL_DIR) $(DESTDIR)$(bindir)", NULL);
      }
      var_set("INSTALL_EXEC", str_start(v, "install") ? "$(INSTALL) -m 755" : "$(INSTALL)");
      stralloc_catm_internal(
          &inst->recipe, newline, "\t$(INSTALL_EXEC) ", MAP_ITER_KEY(t), " $(DESTDIR)$(bindir)", NULL);
    }
    if(do_lib) {
      if(!var_isset("libdir")) {
        var_set("libdir", "$(prefix)/lib");
        if(str_end(tools.compiler, "64")) {
          var_push("libdir", "$(X64)");
        }
        stralloc_catm_internal(&inst->recipe, newline, "\t$(INSTALL_DIR) $(DESTDIR)$(libdir)", NULL);
      }
      stralloc_catm_internal(
          &inst->recipe, newline, "\t$(INSTALL_DATA) ", MAP_ITER_KEY(t), " $(DESTDIR)$(libdir)", NULL);
    }
  }
  return inst;
}

void
get_keys(MAP_T* map, strlist* list) {
  MAP_PAIR_T t;
  MAP_FOREACH(*map, t) { strlist_push(list, MAP_ITER_KEY(t)); }
}

void
input_process_path_b(const char* y, size_t len, stralloc* out) {
#ifdef DEBUG_OUTPUT_
  buffer_puts(buffer_2, "input_process_path_b \"");
  buffer_put(buffer_2, y, len);
  buffer_puts(buffer_2, "\"");
  buffer_putnlflush(buffer_2);
#endif

  stralloc_init(out);
  path_append(dirs.build.sa.s, dirs.build.sa.len, out);
  path_append(y, len, out);
  stralloc_nul(out);
  path_relative_to(out->s, dirs.out.sa.s, out);
  path_prepends(".", out);

  path_collapse_sa(out);
  stralloc_nul(out);
#ifdef DEBUG_OUTPUT_
  buffer_puts(buffer_2, "out \"");
  buffer_puts(buffer_2, out->s);
  buffer_puts(buffer_2, "\"");
  buffer_putnlflush(buffer_2);
#endif
}

void
input_process_path(const char* y, stralloc* out) {
  input_process_path_b(y, str_len(y), out);
}

int
input_process_command(stralloc* cmd, int argc, char* argv[], const char* file, size_t line) {
  size_t n, len;
  int compile = 0, link = 0, lib = 0, objects = 0;
  const char *x, *y;
  char **p, **end = argv + argc;
  stralloc output, dir, path;
  strlist args, files, flags, libs;

  stralloc reldir = {0, 0, 0};
  path_relative_to_b(dirs.out.sa.s, dirs.out.sa.len, dirs.build.sa.s, dirs.build.sa.len, &reldir);

  stralloc_init(&output);
  stralloc_init(&dir);
  strlist_init(&args, ' ');
  strlist_init(&files, ' ');
  strlist_init(&flags, ' ');
  strlist_init(&libs, ' ');

  for(int i = 0; i < argc; i++) {
    if(is_source(argv[i])) {
      compile = 1;
      break;
    }
  }
  if(compile || link || lib) {

  } else if(!is_command_b(cmd->s, cmd->len))
    return 0;

  strlist_push_sa(&args, cmd);

  if(stralloc_ends(cmd, "make")) {
    for(p = argv; (len = *p ? str_len(*p) : 0, x = *p, p < end); p++) {
      len = str_len(x);
      if(len >= 2 && !byte_diff(x, 2, "-f")) {
        if(len > 2) {
          x += 2;
          len -= 2;
        } else {
          x = *++p;
          len = str_len(x);
        }
        if(len) {
          strlist_pushb(&files, x, len);
          strlist_push(&flags, "-f");
          strlist_pushb(&flags, x, len);
        }
        continue;
      }
      if(is_var(x))
        continue;
      if(output.len)
        stralloc_catc(&output, ' ');
      stralloc_cats(&output, x);
      strlist_push(&flags, x);
    }
  } else {
    for(p = argv; (len = *p ? str_len(*p) : 0, x = *p, p < end); p++) {
      int src = is_source_b(x, len);
      if(byte_equal(x, 2, "-c") || src) {
        compile = 1;
        link = 0;
      }
      if(src)
        sources_add_b(x, byte_chrs(x, len, " \t\n\v\r", 5), &dirs.this.sa);
    }
    for(p = argv; (len = *p ? str_len(*p) : 0, x = *p, p < end); p++) {
      len = str_len(x);
      if(len >= 2 && !byte_diff(x, 2, "-o")) {
        if(len > 2) {
          x += 2;
          len -= 2;
        } else {
          x = *++p;
          len = str_len(x);
        }
        if(len) {
          strlist_replaceb(&args, x, len, "$@", 2);
          stralloc_copyb(&output, x, str_chrs(x, "\r\n", 2));
        }
      }
    }
    for(p = argv; (len = *p ? str_len(*p) : 0, x = *p, p < end); p++) {
      len = str_len(x);
      if(len >= 2 && x[0] == '>') {
        y = x + 1 + (x[1] == '>');
        stralloc_copyb(&output, y, str_chrs(y, "\r\n", 2));
      } else if(len >= 2 && byte_equal(x, 2, "-o")) {
        y = (x[2] == '\0') ? *++p : x + 2;
        input_process_path(y, &path);
        strlist_push(&args, "-o");
        strlist_push(&args, path.s);
        strlist_push(&flags, "-o");
        strlist_push(&flags, path.s);
        stralloc_copyb(&output, y, str_chrs(y, "\r\n", 2));
        stralloc_free(&path);
        continue;
      } else if(len >= 2 && byte_equal(x, 2, "-M")) {
        switch(x[2]) {
          case 'D':
          case 'P':
          case 'M': break;
          case 'T':
          case 'F': {
            y = x + ((x[2] == '\0') ? 3 : 2);
            ++p;
            break;
          }
        }
        continue;
      } else if(len >= 2 && byte_equal(x, 2, "-D")) {
        y = x + ((x[2] == '\0') ? 3 : 2);
        push_define(y);
      } else if(len >= 2 && byte_equal(x, 2, "-I")) {
        size_t i = (x[2] == '\0') ? 3 : 2;
        size_t n = len - i;
        y = x + i;
        input_process_path(y, &path);
        strlist_push(&args, "-I");
        stralloc_cats(&args.sa, path.s);
        strlist_push(&flags, "-I");
        stralloc_cats(&flags.sa, path.s);

        if(n >= 2 && y[n - 1] == '.' && y[n - 2] == '/')
          n -= 2;
        includes_add_b(y, n);
        x = y;
        stralloc_free(&path);
        continue;
      } else if(len >= 2 && byte_equal(x, 2, "-L")) {
        y = x + ((x[2] == '\0') ? 3 : 2);
        input_process_path(y, &path);
        strlist_push(&args, "-L");
        stralloc_cats(&args.sa, path.s);
        strlist_push(&flags, "-L");
        stralloc_cats(&flags.sa, path.s);
        libdirs_add(y);
        x = y;
        stralloc_free(&path);
        continue;
      } else if(len >= 2 && byte_equal(x, 2, "-l")) {
        y = x + ((x[2] == '\0') ? 3 : 2);
        strlist_push(&libs, x);
      } else if(len >= 3 && byte_equal(x, 3, "-MF")) {
        size_t i = len > 3 ? 3 : 4;
        y = x + i;
        len -= i;
      } else if(len >= 2 && byte_equal(x, 2, "-c")) {
        compile = true;
      } else if(len >= 7 && byte_equal(x, 7, "--chip=")) {
        stralloc_copys(&cfg.chip, &x[7]);
      } else if(len == 15 && byte_equal(x, 15, "--memorysummary")) {
        y = x + 16;
        var_set("MEMORYSUMMARY", y);
        strlist_push(&flags, "$(MEMORYSUMMARY)");
        ++p;
      } else if(len >= 1 && x[0] != '-') {
        y = x;
#ifdef DEBUG_OUTPUT_
        buffer_puts(buffer_2, "File \"");
        buffer_put(buffer_2, y, len);
        buffer_puts(buffer_2, "");
        buffer_putnlflush(buffer_2);
#endif
        if(is_filename_b(y, len)) {
          input_process_path_b(y, len, &path);

          strlist_push_sa(&files, &path);
          strlist_push_sa(&args, &path);
#ifdef DEBUG_OUTPUT_
          buffer_puts(buffer_2, "File '");
          buffer_put(buffer_2, path.s, path.len);
          buffer_puts(buffer_2, "'");
          buffer_putnlflush(buffer_2);
#endif
          if(is_source_sa(&path))
            sources_add_b(path.s, path.len, &dirs.this.sa);
          stralloc_free(&path);
        }
        x = y;
        continue;
      }

      strlist_push(&args, x);
      strlist_push(&flags, x);
      x = y;
    }
  }

  if(output.len) {
    strlist_removeb(&files, output.s, output.len);
    path_canonical_sa(&output);
    strlist_removeb(&files, output.s, output.len);
  }

  stralloc_copy(cmd, &args.sa);
  stralloc_nul(cmd);

#ifdef DEBUG_OUTPUT_
  buffer_puts(buffer_2, "\treldir = ");
  buffer_putsa(buffer_2, &reldir);
  buffer_puts(buffer_2, "\n\tbuilddir = ");
  buffer_putsa(buffer_2, &dirs.build.sa);
  buffer_puts(buffer_2, "\n\toutdir = ");
  buffer_putsa(buffer_2, &dirs.out.sa);
  buffer_puts(buffer_2, "\n\tcmd = ");
  buffer_putsa(buffer_2, cmd);
  buffer_puts(buffer_2, "\n\targs =\n\t\t");
  buffer_putsl(buffer_2, &args, "\n\t\t");
  buffer_puts(buffer_2, "\n\tfiles =\n\t\t");
  buffer_putsl(buffer_2, &files, "\n\t\t");
  buffer_putnlflush(buffer_2);
#endif

  for(p = argv; (len = *p ? str_len(*p) : 0, x = *p); p++) {
    len = str_len(x);
    if(byte_finds(x, len, exts.obj) < len || byte_finds(x, len, exts.lib) < len) {
      objects++;
      if(!compile && !lib) {
        if(objects > 1) {
          compile = 0;
          link = 1;
        }
      }
    }
  }
  if(output.len == 0) {
    stralloc sa;
    stralloc_init(&sa);
    // path_normalize_b(files.sa.s, byte_chr(files.sa.s, files.sa.len, files.sep), &sa);
    stralloc_copyb(&sa, files.sa.s, byte_chr(files.sa.s, files.sa.len, files.sep));
    stralloc_nul(&sa);
    if(compile)
      path_extension(sa.s, &output, exts.obj);
    stralloc_free(&sa);
  }
  if(!output.len) {
    n = count_b(&files, &is_source_b);
    if(n < strlist_count(&files)) {
      stralloc sa;
      stralloc_init(&sa);
      if(strlist_copyat(&files, n, &output)) {
        strlist_removeb(&files, output.s, output.len);
        stralloc_nul(&output);
      }
      stralloc_free(&sa);
    }
  }
  strlist_removeb(&files, output.s, output.len);

  if(stralloc_starts(&output, "./"))
    stralloc_remove(&output, 0, 2);

  stralloc_nul(&output);

  /* if(link && stralloc_ends(&output, exts.lib)) {
     link = 0;
     lib = 1;
   }*/

#ifdef DEBUG_OUTPUT_
  buffer_puts(buffer_2, file);
  buffer_puts(buffer_2, ":");
  buffer_putulong(buffer_2, line);
  buffer_puts(buffer_2, ",\n\tobjects = ");
  buffer_putulong(buffer_2, objects);
  buffer_puts(buffer_2, ",\n\toutput = ");
  buffer_putsa(buffer_2, &output);
  buffer_puts(buffer_2, ",\n\tcmd = ");
  buffer_putsa(buffer_2, cmd);
  buffer_puts(buffer_2, ",\n\tfiles =\n\t\t");
  buffer_putsl(buffer_2, &files, ",\n\t\t");
  buffer_putnlflush(buffer_2);
#endif

  if(output.len) {
    uint32* count_ptr;
    stralloc_nul(&output);
    stralloc_zero(&dir);
    path_dirname(output.s, &dir);
    if(!(count_ptr = MAP_GET(targetdirs, dir.s, dir.len + 1))) {
      uint32 count = 0;
      MAP_INSERT(targetdirs, dir.s, dir.len + 1, &count, sizeof(count));
      count_ptr = MAP_GET(targetdirs, dir.s, dir.len + 1);
    }
    (*count_ptr)++;
  }
  n = strlist_count(&files);
  if(output.len) {
    stralloc source;
    stralloc_init(&source);
  }
  stralloc_nul(&output);
  // path_prefix_sa(&dirs.build.sa, &output, pathsep_make);
  n = count_b(&files, &is_source_b);
  if(n > 0)
    compile = 1;
  bool do_rule = (n || count_b(&files, &is_object_b)) || output.len;

  if(stralloc_starts(&output, "@") || stralloc_starts(&output, "/tmp"))
    do_rule = false;

  if(do_rule && (lib || link || compile)) {

#ifdef DEBUG_OUTPUT
    buffer_puts(buffer_2, "Create ");
    buffer_puts(buffer_2, lib ? "lib" : link ? "link" : compile ? "compile" : "other");
    buffer_puts(buffer_2, " RULE\n\toutput = ");
    buffer_putsa(buffer_2, &output);

    buffer_puts(buffer_2, "\n\tcommand = ");
    buffer_put(buffer_2, cmd->s, scan_nonwhitenskip(cmd->s, cmd->len));
    buffer_puts(buffer_2, "\n\tfiles =\n\t\t");
    buffer_putsl(buffer_2, &files, ",\n\t\t");
    buffer_putnlflush(buffer_2);
#endif

    stralloc_catc(cmd, ' ');
    stralloc_cat(cmd, &flags.sa);
    stralloc_nul(cmd);
    {
      stralloc* sacmd;
      size_t pathlen;
      target* rule;
      sacmd = &args.sa;
      /*      if(compile) {
              sacmd = &commands.compile;
            } else if(link) {
              sacmd = &commands.link;
            } else if(lib) {
              sacmd = &commands.lib;
            }*/
      pathlen = reldir.len;
      stralloc_cat(&reldir, &output);

      if((rule = gen_single_rule(&output, sacmd))) {
        rule->type = compile ? COMPILE : lib ? LIB : link ? LINK : 0;

        strlist_foreach(&files, x, n) {
          if(compile && byte_ends(x, n, exts.obj))
            continue;

          if(n >= 2 && byte_equal(x, 2, "./")) {
            x += 2;
            n -= 2;
          }
          set_add(&rule->prereq, x, n);
        }
        strlist_foreach(&libs, x, n) {
          target* dep;
          if((dep = rule_find_lib(x, n, exts.lib, exts.slib))) {
            set_adds(&rule->prereq, dep->name);
            rule_add_dep(rule, dep);
          } else {
            if(n >= 2 && byte_equal(x, 2, "-l")) {
              x += 2;
              n -= 2;
            }
            set_add(&link_libraries, x, n);
          }
        }

#ifdef DEBUG_OUTPUT_
        buffer_puts(buffer_2, "rule = '");
        buffer_puts(buffer_2, rule->name);
        buffer_puts(buffer_2, "', prereq = ");
        buffer_putset(buffer_2, &rule->prereq, " ", 1);
        buffer_putnlflush(buffer_2);
#endif
      }

      /*   if(compile) {
           stralloc_copy(&rule->recipe, &commands.compile);
         } else if(link) {
           stralloc_copy(&rule->recipe, &commands.link);
         } else if(lib) {
           stralloc_copy(&rule->recipe, &commands.lib);
         }*/

      if(link) {
        target* all = rule_get("all");

        set_adds(&all->prereq, rule->name);
      }

#ifdef DEBUG_OUTPUT
      rule_dump(rule);
#endif
    }
  }
  stralloc_free(&output);
  strlist_free(&files);
  return link ? 2 : compile ? 1 : 0;
}

int
input_process_line(const char* x, size_t n, const char* file, size_t line) {
  size_t idx = 0;
  int ret = 0;
  char** av;
  stralloc command;
  strarray args;

#ifdef DEBUG_OUTPUT_
  buffer_puts(buffer_2, "input_process_line \"");
  buffer_put_escaped(buffer_2, x, n, fmt_escapecharshell);
  buffer_puts(buffer_2, "\"");
  buffer_putnlflush(buffer_2);
#endif

  stralloc_init(&command);
  strarray_init(&args);
  while(n > 0) {
    size_t i;
    if((i = scan_whitenskip_escaped(x, n)) == n)
      break;
    x += i;
    n -= i;
    if(n >= 7 && *x == '[') {
      if(n > 16 && !byte_diff(&x[7], 9, "Building ")) {
        i = 16;
      } else if((i = byte_chr(x, n, ']')) < n)
        i++;
      x += i;
      n -= i;
      continue;
    }
    i = scan_nonwhitenskip_escaped(x, n);

    if(idx == 0) {
      if(i >= 8 && byte_equal(x, 8, "depbase=")) {
        i = byte_chr(x, n, ' ');
        if(i < n)
          i++;
        x += i;
        n -= i;
        i = byte_chr(x, n, ' ');
        strarray_pushb(&args, x, i);
        x += i;
        n -= i;
        i = byte_chr(x, n, '\n');
        if(i < n)
          i++;
        x += i;
        n -= i;
        continue;
      }
      if(i >= 2 && x[0] == '"' && x[i - 1] == '"')
        stralloc_copyb(&command, x + 1, i - 2);
      else
        stralloc_copyb(&command, x, i);
    } else if(byte_chrs(x, i, "*?[", 3) < i) {
      bool same_dir;
      stralloc cwd;
      stralloc_init(&cwd);
      path_getcwd(&cwd);
      stralloc_nul(&cwd);
      strlist_nul(&dirs.build);
      same_dir = stralloc_equal(&cwd, &dirs.build.sa);
      if(!same_dir) {
        errno = 0;
        if(chdir(dirs.build.sa.s) == -1)
          errmsg_warnsys("chdir(): ", dirs.build.sa.s, 0);
      }
      strarray_glob_b(&args, x, i);

#ifdef DEBUG_OUTPUT_
      if(strarray_size(&args) >= 1) {
        buffer_puts(buffer_2, "glob = ");
        strarray_dump(buffer_2, &args);
        buffer_putnlflush(buffer_2);
      }
#endif

      if(!same_dir)
        (void)chdir(cwd.s);
      stralloc_free(&cwd);
    } else if(i == 2 && byte_equal(x, 2, "&&")) {
      break;
    } else {
      strarray_pushb(&args, x, i);
    }
    x += i;
    n -= i;
    idx++;
  }

  if(strarray_size(&args) && command.s) {
    if(*(av = strarray_to_argv(&args))) {
      int ac = strarray_size(&args);
      ret = input_process_command(&command, ac, av, file, line);
    }
    if(av)
      alloc_free(av);
  }
  stralloc_free(&command);
  strarray_free(&args);
  return ret;
}

void
input_process_rules(target* all) {
  MAP_PAIR_T t;
  strlist args, builddir, outdir;
  strlist_init(&args, '\0');
  strlist_init(&builddir, PATHSEP_C);
  strlist_init(&outdir, PATHSEP_C);

  stralloc_zero(&commands.compile);

  MAP_DESTROY(vars);
  MAP_NEW(vars);

  MAP_FOREACH(rules, t) {
    const char* name = MAP_ITER_KEY(t);
    target* rule = MAP_ITER_VALUE(t);
    size_t dlen;
    char* dep = set_at_n(&rule->prereq, -1, &dlen);
    bool link = dep ? is_object_b(dep, dlen) : 0;
    bool compile = !link && is_object(name);
    if(compile || link) {
      strlist cmds;
      strlist_init(&cmds, '\0');
      strlist_fromq(&cmds, rule->recipe.s, rule->recipe.len, " \t\r\n", "\"'`");
      if(strlist_count(&args) == 0)
        strlist_copy(&args, &cmds);
      else
        strlist_intersection(&args, &cmds, &args);
      strlist_free(&cmds);
    }
    if(compile) {
      strlist path;
      strlist_init(&path, '\0');
      strlist_froms(&path, rule->name, PATHSEP_C);

      /* if(!builddir.sa.len) */ {
        if(strlist_count(&builddir) == 0)
          strlist_copy(&builddir, &path);
        else
          strlist_intersection(&builddir, &path, compile ? &builddir : &outdir);
      }
      strlist_free(&path);
    }
    if(link) {
      path_dirname(rule->name, &outdir.sa);
    }
#ifdef DEBUG_OUTPUT_
    buffer_putm_internal(buffer_2, "Rule: ", name, link ? " (link)" : compile ? " (compile)" : 0, NULL);
    buffer_putnlflush(buffer_2);
#endif
  }

#ifdef DEBUG_OUTPUT
  buffer_puts(buffer_2, "args: ");
  buffer_putsl(buffer_2, &args, " ");
  buffer_putnlflush(buffer_2);
  buffer_puts(buffer_2, "builddir: ");
  buffer_putsl(buffer_2, &builddir, "/");
  buffer_putnlflush(buffer_2);
#endif

  stralloc_copy(&dirs.work.sa, &builddir.sa);
  if(!stralloc_endc(&dirs.work.sa, PATHSEP_C))
    stralloc_catc(&dirs.work.sa, PATHSEP_C);
  strlist_nul(&dirs.work);

#ifdef DEBUG_OUTPUT_
  buffer_puts(buffer_2, "outdir:\n\t");
  buffer_putsa(buffer_2, &outdir.sa);
  buffer_putnlflush(buffer_2);
#endif

  if(!var_isset(srcdir_varname)) {
    path_relative_b(dirs.out.sa.s, dirs.out.sa.len, &dirs.out.sa);

    var_setb(srcdir_varname, dirs.out.sa.s, dirs.out.sa.len);
  }

  var_t *cflags = var_list("CFLAGS", pathsep_args), *cc = var_list("CC", pathsep_args),
        *defs = var_list("DEFS", pathsep_args), *common = var_list("COMMON_FLAGS", pathsep_args);
  stralloc_zero(&defs->value.sa);

#ifdef DEBUG_OUTPUT
  buffer_puts(buffer_2, "CFLAGS before: ");
  buffer_putsl(buffer_2, &cflags->value, " ");
  buffer_putnlflush(buffer_2);
#endif

  strlist_free(&cflags->value);
  strlist_init(&cflags->value, '\0');
  size_t count = strlist_count(&args);
  strlist_slice(&cflags->value, &args, 1, count);
  strlist_at_sa(&args, &cc->value.sa, 0);
  buffer_puts(buffer_2, "CC before: ");
  buffer_putsl(buffer_2, &cc->value, " ");
  buffer_putnlflush(buffer_2);
  ssize_t found;
  if((found = strlist_match(&args, "--chip=*", 0)) >= 0) {
    char* chip = strlist_at(&args, found);
    chip += str_chr(chip, '=') + 1;
    stralloc_copys(&cfg.chip, chip);
    stralloc_nul(&cfg.chip);
    stralloc_replaces(&cflags->value.sa, cfg.chip.s, "$(CHIP)");
    stralloc_replaces(&cflags->value.sa, cfg.chip.s, "$(CHIP)");
    // stralloc_lower(&cfg.chip);

#ifdef DEBUG_OUTPUT
    buffer_puts(buffer_2, "Chip: ");
    buffer_putsa(buffer_2, &cfg.chip);
    buffer_putnlflush(buffer_2);
#endif
  }
  strlist_filter(&cflags->value, &defs->value, &cflags->value, "-D*");

  common->value.sep = '\0';
  strlist_filter(&cflags->value, &common->value, &cflags->value, "--*format=*");

  if(common->value.sa.len)
    strlist_push(&cflags->value, "$(COMMON_FLAGS)");

  strlist_nul(&cc->value);
  {
    strlist compiler;
    char* s;
    size_t n;
    strlist_init(&compiler, '/');
    strlist_froms(&compiler, cc->value.sa.s + 1, PATHSEP_C);
    strlist_foreach(&compiler, s, n) {
      if(is_version_b(s, n)) {
        var_setb("VER", s, n);
        stralloc_replace(&cc->value.sa, s - compiler.sa.s + 1, n, "$(VER)", 6);
        break;
      }
    }
  }

#ifdef DEBUG_OUTPUT
  buffer_puts(buffer_2, "CC after: ");
  buffer_putsa(buffer_2, &cc->value.sa);
  buffer_putnlflush(buffer_2);
  buffer_puts(buffer_2, "CFLAGS after: ");
  buffer_putsl(buffer_2, &cflags->value, " ");
  buffer_putnlflush(buffer_2);
#endif

  /*  MAP_FOREACH(rules, t) {
      target* rule = MAP_ITER_VALUE(t);
      stralloc* sa = &commands.v[rule->type];
      if(sa->s && sa->len)
        stralloc_copy(&rule->recipe, sa);
    }
  */
  MAP_FOREACH(rules, t) {
    const char* name = MAP_ITER_KEY(t);
    target* rule = MAP_ITER_VALUE(t);
    size_t dlen;
    char* dep = set_at_n(&rule->prereq, 0, &dlen);
    bool link = dep ? is_object_b(dep, dlen) : false;
    bool compile = !link && is_object(name);
    if(compile || link) {
      char* x;
      size_t i, n;
      set_iterator_t it;
      stralloc cmd;
      stralloc_init(&cmd);
      stralloc_copy(&cmd, &rule->recipe);

      if(link && (i = stralloc_findb(&cmd, "\n", 1)) < cmd.len)
        stralloc_trunc(&cmd, i);

      stralloc_0(&cmd);

      strlist_foreach(&args, x, n) { REMOVE(&cmd, i, x, n); }

      set_foreach(&rule->prereq, it, x, n) { REMOVE(&cmd, i, x, n); }
      stralloc_0(&cmd);

#ifdef DEBUG_OUTPUT_
      buffer_puts(buffer_2, "Prereq: ");
      buffer_putset(buffer_2, &rule->prereq, " ", 1);
      buffer_putnlflush(buffer_2);
#endif

      stralloc_prepends(&cmd, "$(CC) $(CFLAGS) ");
      if(cmd.len && cmd.s[cmd.len - 1] != ' ' && cmd.s[cmd.len - 1] != PATHSEP_C)
        stralloc_catc(&cmd, ' ');
      stralloc_cats(&cmd, set_size(&rule->prereq) > 1 ? "$^" : "$<");
      if((i = stralloc_finds(&cmd, name)) < cmd.len) {
        stralloc_replace(&cmd, i, str_len(name), "$@", 2);
      }

      if(compile) {
        if(commands.compile.len == 0 /* && !infile*/)
          stralloc_copy(&commands.compile, &cmd);

        /*    stralloc_copy(&rule->recipe, &commands.compile); */
        stralloc_nul(&rule->recipe);
      }

      stralloc_free(&cmd);
    }
#ifdef DEBUG_OUTPUT_
    buffer_putm_internal(buffer_2, "Rule: ", name, compile ? " (compile)" : link ? " (link)" : "", "\n\t", NULL);
    buffer_putsa(buffer_2, &rule->recipe);
    buffer_putnlflush(buffer_2);
#endif
  }
}

void
input_process(const char* infile, target* all) {
  const char* x;
  size_t n, line = 1;
  path_dirname(infile, &dirs.this.sa);
  strlist_nul(&dirs.this);
  // sources_get(dirs.this.sa.s);
  if((x = mmap_read(infile, &n))) {
    while(n > 0) {
      size_t i = scan_lineskip_escaped(x, n);
      size_t ln = byte_count(x, i, '\n');
      if(!(i > 2 && x[0] == '-' && x[1] == '-')) {
        size_t pos;
        if((pos = byte_finds(x, i, "ing directory '")) < i) {
          bool enter = byte_equal(&x[pos - 5], 5, "Enter");
          size_t len;
          pos += 15;
          len = byte_chr(&x[pos], i - 15, '\'');
          if(enter)
            builddir_enter(&x[pos], len);
          else
            builddir_leave(&x[pos], len);
        } else if(i > 0) {
          size_t len = i;
          if(len > 0 && x[len - 1] == '\n')
            --len;
          if(len > 0 && x[len - 1] == '\r')
            --len;
          input_process_line(x, len, infile, line);
        }
      }

      x += i;
      n -= i;
      line += ln;
    }
    mmap_unmap(x, n);
  }

  input_process_rules(all);
}

/**
 * @brief output_all_vars  Output all
 * variables
 * @param b
 * @param vars
 */
void
output_var(buffer* b, MAP_T* vars, const char* name, int serial) {
  stralloc v;
  var_t* var;
  set_t refvars;
  const char* ref;
  size_t len;
  set_iterator_t it;
  MAP_PAIR_T t = 0;
  set_init(&refvars, 0);
  if(MAP_SEARCH(*vars, name, str_len(name) + 1, &t)) {
    var = MAP_ITER_VALUE(t);
    if(var->serial == serial)
      return;
    stralloc_init(&v);
    if(var->value.sa.len) {
      stralloc_copys(&v, MAP_ITER_KEY(t));
      if(ninja)
        stralloc_lower(&v);
      stralloc_nul(&v);
      set_clear(&refvars);
      if(!infile) {
        extract_vars(var->value.sa.s, var->value.sa.len, &refvars);
        set_foreach(&refvars, it, ref, len) {
#ifdef DEBUG_OUTPUT
          buffer_putm_internal(buffer_2, "Var ", name, " ref: ", ref, NULL);
          buffer_putnlflush(buffer_2);
#endif
          output_var(b, vars, ref, serial);
        }
      }
      if(batch)
        buffer_putm_internal(b, "@SET ", v.s, "=", NULL);
      else if(shell)
        buffer_putm_internal(b, v.s, "=\"", NULL);
      else
        buffer_putm_internal(b, v.s, " = ", NULL);
      var->serial = serial;
      stralloc u;
      stralloc_init(&u);
      if(ninja)
        stralloc_copy(&u, &var->value.sa);
      else
        strlist_joinq(&var->value, &u, ' ', '"');

      if(ninja || shell) {
        stralloc_zero(&v);
        var_subst(&u, &v, "$", "", 1);
      } else if(batch) {
        stralloc_zero(&v);
        var_subst(&u, &v, "%", "%", 1);
      } else {
        stralloc_copy(&v, &u);
      }
      stralloc_free(&u);
      buffer_putsa(b, &v);
      if(shell)
        buffer_putc(b, '"');
      put_newline(b, 0);
      buffer_flush(b);
    }
    stralloc_free(&v);
    MAP_DELETE(*vars, MAP_ITER_KEY(t), str_len(MAP_ITER_KEY(t)));
  }
  set_free(&refvars);
}

/**
 * @brief output_all_vars  Output all
 * variables
 * @param b
 * @param vars
 */
void
output_all_vars(buffer* b, MAP_T* vars, strlist* varnames) {
  const char* name;
  static int serial = 0;
  stralloc_nul(&varnames->sa);
  ++serial;
  strlist_foreach_s(varnames, name) { output_var(b, vars, name, serial); }
  put_newline(b, 1);
}

/**
 * @brief output_make_rule  Output rule
 * to buffer
 * @param b
 * @param rule
 */

void
output_make_rule(buffer* b, target* rule) {
  const char* x;
  static stralloc output, sa, name;
  size_t n, num_prereqs;
  num_prereqs = set_size(&rule->prereq);
  debug_nl = "\n";
  stralloc_zero(&name);
  stralloc_copys(&name, rule->name);

#ifdef DEBUG_OUTPUT
  buffer_puts(buffer_2, "RULE\n  " YELLOW256 "name" NC ": ");
  buffer_puts(buffer_2, rule->name);
  buffer_puts(buffer_2, "\n  " PURPLE256 "recipe" NC ": ");
  buffer_putsa(buffer_2, &rule->recipe);
  buffer_puts(buffer_2, "\n  " BLUE256 "output" NC ":\n\t");
  buffer_putset(buffer_2, &rule->output, "\n\t", 2);
  buffer_puts(buffer_2, "\n  " GREEN256 "prereq" NC ":\n\t");
  buffer_putset(buffer_2, &rule->prereq, "\n\t", 2);
  if(array_length(&rule->deps, sizeof(target*))) {
    buffer_puts(buffer_2, "\n  deps" NC ": ");
    print_rule_deps(buffer_2, rule);
  }
  buffer_putnlflush(buffer_2);
#endif

  if(rule->phony || (num_prereqs == 0 && str_diffn(rule->name, dirs.work.sa.s, dirs.work.sa.len) &&
                     !rule->name[str_chr(rule->name, pathsep_make)] && str_end(rule->name, ":"))) {
    buffer_putm_internal(b, ".PHONY: ", rule->name, newline, NULL);
  }
  stralloc_zero(&sa);
  stralloc_catset(&sa, &rule->output, " \\\n");

#ifdef DEBUG_OUTPUT_
  buffer_puts(buffer_2, "RULE OUTPUT: ");
  buffer_putsa_escaped(buffer_2, &sa, &fmt_escapecharcontrol);
  buffer_putnlflush(buffer_2);
#endif
  strlist_foreach(&rule->vars, x, n) {
    // buffer_puts(b, "output: ");
    buffer_putsa(b, &sa);
    buffer_puts(b, ": ");
    buffer_put(b, x, n);
    buffer_puts(b, "\n\n");
  }
  stralloc_zero(&output);
  stralloc_nul(&output);

  if(str_contains(rule->name, "%")) {
    stralloc_cats(&sa, ": ");
    stralloc_cats(&sa, rule->name);

  } else {
    if(!set_has_s(&rule->output, rule->name)) {
      stralloc_cats(&sa, " \\\n");
      stralloc_cats(&sa, rule->name);
    }
  }

  if(sa.len > 0 && rule->outputs)
    stralloc_cat(&output, &sa);
  else
    stralloc_cats(&output, rule->name);

  stralloc_catc(&output, ':');

  if(num_prereqs) {
    const char* str;
    size_t len;
    bucket_t* it;
    int i = 0;
    stralloc_cats(&output, num_prereqs > 1 ? " \\\n\t" : " ");

    /*set_join(&rule->prereq, num_prereqs > 1 ? " \\\n\t" : " ", &output);*/
    set_foreach_ordered(&rule->prereq, it, str, len) {
      if(stralloc_endb(&output, str, len))
        continue;
      if(i)
        stralloc_cats(&output, num_prereqs > 1 ? " \\\n\t" : " ");
      stralloc_catb(&output, str, len);
      i++;
    }
    // stralloc_replacec(&output, pathsep_make == '/' ? '\\' : '/', pathsep_make);
  }

  // stralloc_zero(&output);
  if(rule->recipe.s) {
    stralloc cmd;
    stralloc_init(&cmd);

    if(infile && (signed)rule->type >= 0)
      stralloc_copy(&cmd, &commands.v[rule->type]);
    else
      rule_command(rule, &cmd, shell, batch, quote_args, pathsep_args, make_sep_inline, tools.make);

    stralloc_remove_all(&cmd, "\0", 1);

    if(!stralloc_starts(&cmd, newline)) {
      stralloc_catc(&output, '\n');
      stralloc_catc(&output, '\t');
    }
    stralloc_cat(&output, &cmd);
    stralloc_catc(&output, '\n');
    if(str_end(rule->name, ":")) {
      stralloc_catc(&output, '\n');
      stralloc_catc(&output, '\n');
      bucket_t* b;
      for(b = rule->prereq.list; b; b = b->list_next) {
        stralloc_catc(&output, ' ');
        stralloc_catb(&output, b->value, b->size);
      }
      stralloc_cats(&output, " :");
      stralloc_catc(&output, '\n');
    }
    stralloc_free(&cmd);
  }
  stralloc_catc(&output, '\n');
  if(str_equal(tools.make, "gmake")) {
    stralloc_replaces(&output, dirs.work.sa.s, "$(BUILDDIR)");
    // stralloc_replaces(&output, dirs.out.sa.s, "$(DISTDIR)");
  }
  buffer_putsa(b, &output);
  buffer_flush(b);
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
  stralloc source_file, obj_dir;

  if(rule_is_compile(rule) || rule->recipe.s == commands.compile.s)
    rule_name = "cc";
  else if(rule_is_link(rule) || rule->recipe.s == commands.link.s)
    rule_name = "link";
  else if(rule_is_lib(rule) || stralloc_equal(&rule->recipe, &commands.lib))
    rule_name = "lib";
  if(rule_name) {
    stralloc path;
    stralloc_init(&path);
    set_at_sa(&rule->output, 0, &path);

    stralloc_replaces(&path, dirs.build.sa.s, "$objdir");
    // stralloc_replaces(&path, dirs.out.sa.s, "$distdir");

    /*stralloc_subst(
        &path, rule->name, str_len(rule->name), pathsep_args == '/' ? "\\" : "/", pathsep_args == '/' ? "/" : "\\");*/
    buffer_puts(b, "build ");
    buffer_putsa(b, &path);
    buffer_puts(b, ": ");
    buffer_puts(b, rule_name);
    buffer_puts(b, " ");
    stralloc_zero(&path);
    stralloc_init(&source_file);
    stralloc_init(&obj_dir);
    path_relative_to_sa(&dirs.out.sa, &dirs.work.sa, &obj_dir);
    {
      const char* x;
      size_t n, i = 0;
      set_iterator_t it;
      stralloc tmp, outdir;
      stralloc_init(&tmp);
      stralloc_init(&outdir);
      path_concat_sa(&dirs.this.sa, &dirs.out.sa, &outdir);

      set_foreach(&rule->prereq, it, x, n) {
        if(i)
          stralloc_catc(&path, ' ');
        /*     path_concatb(dirs.this.sa.s, dirs.this.sa.len, x, n, &source_file);

             path_relative_to_sa(&source_file, &dirs.build.sa, &tmp);
             stralloc_cat(&path, &tmp);
     */
        stralloc_catb(&path, x, n);
        stralloc_zero(&tmp);
        stralloc_zero(&source_file);
        i++;
      }
      stralloc_free(&tmp);
      stralloc_free(&outdir);
    }
    stralloc_free(&source_file);
    stralloc_nul(&path);

    // stralloc_catset(&path, &rule->prereq, " ");
    stralloc_replacec(&path, pathsep_args == '/' ? '\\' : '/', pathsep_args == '/' ? '/' : '\\');
    stralloc_replaces(&path, dirs.build.sa.s, "$objdir");
    // stralloc_replaces(&path, dirs.out.sa.s, "$distdir/");

    buffer_putsa(b, &path);
    buffer_putnlflush(b);
    stralloc_free(&obj_dir);
    stralloc_free(&path);
  }
}

/**
 * @brief output_all_rules  Output the
 * rule set
 * @param b
 * @param hmap
 */
void
output_all_rules(buffer* b) {
  MAP_PAIR_T t;
  MAP_FOREACH(rules, t) {
    // target* rule = MAP_ITER_VALUE(t);
    const char* name = MAP_ITER_KEY(t);
    target* rule = MAP_ITER_VALUE(t);
    if(!cmd_libs && str_end(name, ".a"))
      continue;

    if(rule->disabled)
      continue;

#ifdef DEBUG_OUTPUT
    buffer_puts(buffer_2, "Outputting rule '");
    buffer_puts(buffer_2, name);
    buffer_putc(buffer_2, '\'');
    buffer_putnlflush(buffer_2);
#endif

    if(ninja)
      output_ninja_rule(b, rule);
    else
      output_make_rule(b, rule);
  }
}

/**
 * @brief output_build_rules
 * @param b
 * @param name
 * @param cmd
 */
void
output_build_rules(buffer* b, const char* name, const stralloc* cmd) {
  static stralloc out;
  stralloc_zero(&out);
  buffer_putm_internal(b, "rule ", name, "\n  command = ", NULL);
  var_subst(cmd, &out, "$", "", 1);
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
 * @brief output_script
 * @param b
 * @param rule
 */
void
output_script(buffer* b, target* rule) {
  static uint32 serial;
  char* x;
  size_t n;
  set_iterator_t it;
  int flush = 0;
  if(rule == NULL) {
    // MAP_PAIR_T t;
    ++serial;
    /*    MAP_FOREACH(rules, t) {
          rule = MAP_ITER_VALUE(t);
          output_script(b, rule);
        }
    */
    flush = 1;
    rule = rule_get("all");
  }
  if(rule->serial == serial)
    return;
  if(!rule->name[str_chr(rule->name, '%')]) {
    if(rule->recipe.s != commands.compile.s)
      buffer_putm_internal(b, newline, "REM Rules for '", rule->name, "'", newline, NULL);
  }
  set_foreach(&rule->prereq, it, x, n) {
    target* dep = rule_find_b(x, n);
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
      // if(dep->name[str_chr(dep->name,'%')])
      output_script(b, dep);
    }
  }
  if(rule->recipe.len) {
    stralloc cmd;
    stralloc_init(&cmd);
    rule_command(rule, &cmd, shell, batch, quote_args, pathsep_args, make_sep_inline, tools.make);
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
 * @brief set_machine  Set the machine
 * type
 * @param s
 * @return
 */
int
set_machine(const char* s) {
  int ret = 1;
  if(s[str_find(s, "64")])
    cfg.mach.bits = _64;
  else if(s[str_find(s, "32")])
    cfg.mach.bits = _32;
  else if(s[str_find(s, "386")])
    cfg.mach.bits = _32;
  else
    ret = 0;
  if(str_start(s, "pic"))
    cfg.mach.arch = PIC;
  else if(s[str_find(s, "arm")] || s[str_find(s, "aarch")])
    cfg.mach.arch = ARM;
  else if(s[str_find(s, "86")])
    cfg.mach.arch = X86;
  else
    ret = 0;
  return ret;
}

/**
 * @brief set_chip  Set the system type
 * @param s
 * @return
 */
int
set_chip(const char* s) {
  int ret = 1;
  size_t pos = 0;
  if(s[(pos = str_find(s, "16f"))] || s[(pos = str_find(s, "16F"))]) {
    cfg.mach.arch = PIC;
    cfg.mach.bits = _14;
  } else if(s[(pos = str_find(s, "18f"))] || s[(pos = str_find(s, "18F"))]) {
    cfg.mach.arch = PIC;
    cfg.mach.bits = _16;
  }
  stralloc_copys(&cfg.chip, &s[pos]);
  return ret;
}

/**
 * @brief set_system  Set the system
 * type
 * @param s
 * @return
 */
int
set_system(const char* s) {
  int ret = 1;
  if(str_contains(s, "win") || str_contains(s, "mingw")) {
    cfg.sys.os = OS_WIN;
    cfg.sys.type = NTOS;
  } else if(s[str_find(s, "msys")] || s[str_find(s, "cygwin")]) {
    cfg.sys.os = OS_WIN;
    cfg.sys.type = UNIX;
  } else if(s[str_find(s, "mac")]) {
    cfg.sys.os = OS_MAC;
    cfg.sys.type = UNIX;
  } else if(s[str_find(s, "lin")]) {
    cfg.sys.os = OS_LINUX;
    cfg.sys.type = UNIX;
  } else {
    ret = 0;
  }
  // pathsep_args = cfg.sys.type == NTOS
  //? '\\' : '/'; pathsep_make =
  // cfg.sys.type == NTOS ? '\\' : '/';
  return ret;
}

/**
 * @brief set_make_type  Set tools.make
 * program type
 * @param tools.make
 * @param tools.compiler
 * @return
 */
int
set_make_type() {
  const char* inst = "install";

#if WINDOWS_NATIVE
  newline = "\r\n";
#else
  newline = "\n";
#endif

  stralloc_copys(&commands.mkdir, cfg.sys.os == OS_WIN ? "IF NOT EXIST $@ MKDIR $@" : "mkdir -p $@");
  if(str_start(tools.make, "batch") || str_start(tools.make, "cmd")) {
    pathsep_args = '\\';
    pathsep_make = '\\';
  } else if(str_start(tools.make, "bmake") || str_start(tools.make, "borland")) {
    /* Borland LANG_C++ Builder Make */
    pathsep_make = '\\';
    make_begin_inline = "@&&|\r\n ";
    make_sep_inline = " ";
    make_end_inline = "\r\n|";
    inst = "COPY /Y";
    newline = "\r\n";
  } else if(str_start(tools.make, "nmake")) {
    /* Microsoft NMake */
    pathsep_make = '\\';
    pathsep_args = '\\';
    // make_begin_inline = "@<<$*.rsp\r\n";
    // make_end_inline = "\r\n<<keep";
    stralloc_copys(&commands.mkdir, "IF NOT EXIST $@ MKDIR $@");
    stralloc_copys(&commands.delete, "DEL /F");
    newline = "\r\n";
    inst = "COPY /Y";
  } else if(str_start(tools.make, "gmake") || str_start(tools.make, "gnu")) {
    newline = "\n";
    pathsep_make = '/';
    stralloc_copys(&commands.mkdir, "test -d $@ || mkdir -p $@");
    stralloc_copys(&commands.delete, "rm -f");
  } else if(str_start(tools.make, "omake") || str_start(tools.make, "orange")) {
    pathsep_make = '\\';
    if(inst_bins || inst_libs)
      var_set("INSTALL", "copy /y");
  } else if(str_start(tools.make, "ninja")) {
    ninja = 1;
    pathsep_make = pathsep_args = PATHSEP_C;
    make_begin_inline = make_sep_inline = make_end_inline = 0;
  } else if(str_start(tools.make, "po")) {
    pathsep_make = '\\';
    make_begin_inline = "@<<\r\n ";
    make_end_inline = "\r\n<<";
    inst = "copy /y";
  } else if(str_equal(tools.make, "mplab")) {
  } else if(str_equal(tools.make, "cmake")) {
    builddir_varname = "CMAKE_CURRENT_BINARY_DIR";
    srcdir_varname = "CMAKE_CURRENT_SOURCE_DIR";
  }
  if(inst_bins || inst_libs)
    var_set("INSTALL", inst);
  pathsep_args = pathsep_make;
  return 1;
}

/**
 * @brief set_tools.compiler_type Set
 * the tools.compiler type
 * @param tools.compiler
 * @return
 */
int
set_compiler_type(const char* compiler) {
  var_set("CC", "cc");
  var_set("CXX", "c++");
  stralloc_copys(&commands.compile, "$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(DEFS) -c -o $@ $<");
  set_command(&commands.lib, "$(LIB) /out:$@", "$^");
  set_command(&commands.link,
              "$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(LDFLAGS) $(EXTRA_LDFLAGS) -o $@",
              "$^ $(LIBS) $(EXTRA_LIBS) $(STDC_LIBS)");
  set_command(&commands.preprocess, "$(CPP) $(CPPFLAGS) $(DEFS) -o$@", "$<");
  // var_push("DEFS", "-DHAVE_ERRNO_H=1");
  /*
   * Visual C++ compiler
   */
  if(str_start(compiler, "msvc") || str_start(compiler, "icl") || str_start(compiler, "vs20") ||
     str_start(compiler, "vc") || compiler[str_find(compiler, "-cl")]) {
    exts.obj = ".obj";
    exts.bin = ".exe";
    exts.lib = ".lib";
    var_set("CC", "cl -nologo");
    var_set("LIB", "lib");
    var_set("LINK", "link");
    var_push("CFLAGS", cfg.build_type == BUILD_TYPE_DEBUG ? "-MTd" : "-MT");
    var_push("CPPFLAGS", "-Dinline=__inline");
    if(cfg.build_type == BUILD_TYPE_DEBUG || cfg.build_type == BUILD_TYPE_RELWITHDEBINFO)
      var_push("CFLAGS", "-Zi");
    if(cfg.build_type == BUILD_TYPE_MINSIZEREL)
      var_push("CFLAGS", "-Os");
    else if(cfg.build_type != BUILD_TYPE_DEBUG)
      var_push("CFLAGS", "-Ox");
    // var_push("LDFLAGS", "/DEBUG /DYNAMICBASE /INCREMENTAL /NXCOMPAT /TLBID:1");

    // var_push("LDFLAGS", "/SUBSYSTEM:CONSOLE /TLBID:1 /DYNAMICBASE /NXCOMPAT");
    // var_push("LDFLAGS", "/MANIFEST /manifest:embed2 /MANIFESTUAC:\"level=asInvoker uiAccess=false\"");
    stralloc_copys(&commands.compile, "$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(DEFS) -c -Fo\"$@\" $<");
    set_command(&commands.lib, "$(LIB) -out:$@", "$^");
    // stralloc_copys(&commands.lib, "$(LIB) /OUT:$@ <<\n\t\t$^\n<<");
    /*
     * Intel C++ compiler
     */
    if(str_start(compiler, "icl")) {
      var_set("CC", "icl -nologo");
      var_set("CXX", "icl -nologo");
      var_set("LINK", "xilink");
      var_set("LIB", "xilib");
      var_push("CFLAGS", "-Qip");
      var_push("CFLAGS", "-Qunroll4");
      var_push("CFLAGS", "-Qauto-ilp32");
      if(cfg.mach.bits == _64)
        var_push("LDFLAGS", "-libpath:\"$(ROOT)\\compiler\\lib\\intel64\"");
      else
        var_push("LDFLAGS", "-libpath:\"$(ROOT)\\compiler\\lib\"");
      // stralloc_copys(&commands.compile, "$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(DEFS) -c -Fo $@ $<");
    }
    var_push("LDFLAGS", "-libpath:\"$(UNIVERSALCRTSDKDIR)lib\\$(WINDOWSSDKLIBVERSION)ucrt\\$(MACHINE)\"");
    var_push("LDFLAGS", "-libpath:\"$(WINDOWSSDKDIR)lib\\$(WINDOWSSDKLIBVERSION)um\\$(MACHINE)\"");
    var_push("LDFLAGS", "-libpath:\"$(VCTOOLSINSTALLDIR)lib\\$(MACHINE)\"");
    var_push("LDFLAGS", "-libpath:\"$(WINDOWSSDKDIR)lib$(X64)\"");
    var_push("LDFLAGS", "-libpath:\"$(VCINSTALLDIR)\\lib$(AMD64)\"");
    var_push("LDFLAGS", "-libpath:\"$(VCINSTALLDIR)\\PlatformSDK\\lib$(AMD64)\"");
    var_push("LDFLAGS", "-incremental -manifest");
    if(cfg.build_type == BUILD_TYPE_DEBUG)
      var_push("LDFLAGS", "-debug");
    if(str_start(compiler, "icl"))
      var_push("LDFLAGS", "-manifest:embed -manifestuac:\"level='asInvoker' uiAccess='false'\"");
    if(cfg.mach.arch == ARM) {
      var_push("LDFLAGS", "-machine:ARM");
      var_set("MACHINE", cfg.mach.bits == _64 ? "arm64" : "arm");
    } else if(cfg.mach.bits == _64) {
      var_push("LDFLAGS", "-machine:X64");
      var_set("MACHINE", "x64");
      var_set("X64", "\\x64");
      var_set("AMD64", "\\amd64");
    } else if(cfg.mach.bits == _32) {
      var_push("LDFLAGS", "-machine:X86");
      var_set("MACHINE", "x86");
      var_set("X64", "");
    }
    set_command(&commands.link,
                "$(LINK) -out:$@ $(LDFLAGS) $(EXTRA_LDFLAGS) -pdb:\"$@.pdb\"",
                "$^ $(LIBS) $(EXTRA_LIBS)");
  } else if(str_start(compiler, "gnu") || str_start(compiler, "gcc") || cygming || str_start(compiler, "clang") ||
            str_start(compiler, "llvm") || str_start(compiler, "zapcc")) {
    exts.lib = ".a";
    exts.obj = ".o";
    if(str_start(compiler, "zapcc"))
      var_set("CC", "zapcc");
    if(cfg.build_type == BUILD_TYPE_DEBUG)
      var_set("CFLAGS", "-O0");
    else if(cfg.build_type == BUILD_TYPE_MINSIZEREL)
      var_set("CFLAGS", "-Os");
    else
      var_set("CFLAGS", "-O2");
    if(str_end(compiler, "32"))
      var_push("CFLAGS", "-m32");
    if(str_end(compiler, "64"))
      var_push("CFLAGS", "-m64");
    if(cfg.build_type == BUILD_TYPE_DEBUG || cfg.build_type == BUILD_TYPE_RELWITHDEBINFO) {
      var_push("CFLAGS", "-g");
      var_push("LDFLAGS", "-g");
    }
    /*
     * GNU GCC compatible compilers
     */
    if(str_start(compiler, "gnu") || str_start(compiler, "gcc") || cygming) {
      var_set("CC", "gcc");
      var_set("CXX", "g++");
      var_set("AR", str_start(compiler, "gcc") ? "gcc-ar" : "ar");
      if(cfg.build_type == BUILD_TYPE_DEBUG || cfg.build_type == BUILD_TYPE_RELWITHDEBINFO)
        var_push("CFLAGS", "-ggdb");
    } else if(str_start(compiler, "clang") || str_start(compiler, "llvm")) {
      pathsep_args = '/';
      var_set("CC", "clang");
      var_set("CXX", "clang++");
      var_set("AR", "llvm-ar");
    }
    set_command(&commands.lib, "$(AR) rcs $@", "$^");
    // set_command(&commands.link, "$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(LDFLAGS) $(EXTRA_LDFLAGS) -o $@", "$^ $(LIBS)
    //$(EXTRA_LIBS)");
    set_command(&commands.link, "$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(LDFLAGS) $(EXTRA_LDFLAGS) -o $@", "$^ $(LIBS)");
    exts.bin = "";
    format_linklib_fn = &format_linklib_switch;
    /*
     * Borland C++ Builder
     */
  } else if(str_start(compiler, "bcc")) {
    // pathsep_args = '\\';
    // var_push("DEFS", "-DWIN32_LEAN_AND_MEAN");
    if(cfg.build_type == BUILD_TYPE_MINSIZEREL)
      var_set("CFLAGS", "-O1");
    else if(cfg.build_type == BUILD_TYPE_RELEASE || cfg.build_type == BUILD_TYPE_RELWITHDEBINFO)
      var_set("CFLAGS", "-O -O2");
    // var_push("CFLAGS", "-q");
    var_push("CFLAGS", "-tWC -tWM");
    var_push("CPPFLAGS", "-Dinline=__inline");
    var_push("LDFLAGS", "-q");
    if(cfg.build_type == BUILD_TYPE_DEBUG)
      var_push("CFLAGS", "-w");
    if(cfg.build_type == BUILD_TYPE_MINSIZEREL)
      var_push("CFLAGS", "-d -a-");
    /* Embracadero C++ */
    if(!str_contains(compiler, "55") && !str_contains(compiler, "60")) {
      var_set("CC", "bcc32c");
      var_set("CXX", "bcc32x");
      /* C99 standard */
      var_push("CFLAGS", "-An");
      if(cfg.build_type == BUILD_TYPE_DEBUG || cfg.build_type == BUILD_TYPE_RELWITHDEBINFO)
        var_push("CFLAGS", "-v");
      /*  if(cfg.build_type !=
         BUILD_TYPE_DEBUG)
          var_push("CFLAGS", "-Or");
      */
      set_command(&commands.link, "$(CC) $(LDFLAGS) $(EXTRA_LDFLAGS) -o $@ ", "$^ $(LIBS) $(EXTRA_LIBS) $(STDC_LIBS)");
      /* Borland C++ Builder 5.5 */
    } else {
      var_set("CC", "bcc32");
      var_set("CXX", "bcc32");
      var_push("CFLAGS", "-ff -fp");
      if(cfg.build_type == BUILD_TYPE_DEBUG || cfg.build_type == BUILD_TYPE_RELWITHDEBINFO)
        var_push("CFLAGS", "-y");
      if(cfg.build_type == BUILD_TYPE_DEBUG || cfg.build_type == BUILD_TYPE_RELWITHDEBINFO) {
        var_push("CFLAGS", "-v");
        var_push("LDFLAGS", "-v");
      }
      if(cfg.build_type == BUILD_TYPE_DEBUG)
        var_push("CFLAGS", "-w-use");
      else
        var_push("CFLAGS", "-r");
      stralloc_copys(&commands.compile, "$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(DEFS) -c -o$@ $<");
      set_command(&commands.link, "$(CC) $(LDFLAGS) $(EXTRA_LDFLAGS) -e$@", "$^ $(LIBS) $(EXTRA_LIBS) $(STDC_LIBS)");
    }
    var_set("LINK", "ilink32");
    var_set("LIB", "tlib");
    push_lib("STDC_LIBS", "cw32");
    push_lib("STDC_LIBS", "import32");
    set_command(&commands.lib, "$(LIB) /p256 $@ /u", "$^");
    quote_args = "\"";
    pathsep_args = '\\';
    /*
     * LCC compiler
     */
  } else if(str_start(compiler, "lcc")) {
    if(cfg.mach.bits == _64) {
      var_set("CC", "lcc64");
      var_set("LINK", "lcclnk64");
      var_set("LIB", "lcclib64");
    } else {
      var_set("CC", "lcc");
      var_set("LINK", "lcclnk");
      var_set("LIB", "lcclib");
    }
    if(cfg.build_type == BUILD_TYPE_DEBUG)
      var_push("CFLAGS", "-g2");
    make_begin_inline = 0;
    make_end_inline = 0;
    // var_push("STDC_LIBS", "oldnames.lib");
    if(cfg.mach.bits == _64) {
      var_push("STDC_LIBS", "ccl64.lib");
      var_push("STDC_LIBS", "libc64.lib");
    } else {
      var_push("STDC_LIBS", "libc.lib");
    }
    stralloc_copys(
        &commands.link,
        "$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(LDFLAGS) $(EXTRA_LDFLAGS) -o $@ $^ $(LIBS) $(EXTRA_LIBS) $(STDC_LIBS)");
    stralloc_copys(&commands.link, "$(LINK) $(LDFLAGS) $(EXTRA_LDFLAGS) -o $@ $^ $(LIBS) $(EXTRA_LIBS) $(STDC_LIBS)");
    /*
     * Tiny CC compiler
     */
  } else if(str_start(compiler, "tcc")) {
    exts.lib = ".a";
    exts.obj = ".o";
    format_linklib_fn = &format_linklib_switch;
    var_set("CC", "tcc");
    var_set("AR", "$(CC) -ar");
    if(cfg.build_type == BUILD_TYPE_DEBUG || cfg.build_type == BUILD_TYPE_RELWITHDEBINFO)
      var_push("CFLAGS", "-g");
    // var_push("LDFLAGS",  "-Wl,-subsystem=console");
    if(cfg.build_type == BUILD_TYPE_MINSIZEREL)
      // var_push("LDFLAGS", "-Wl,-file-alignment=16");
      var_push("CFLAGS", "-Wall");
    var_push("CPPFLAGS", "-D__TCC__=1");
    set_command(&commands.lib, "$(AR) r $@", "$^");
    set_command(&commands.link, "$(CC) $(LDFLAGS) $(EXTRA_LDFLAGS) -o $@", "$^ $(LIBS) $(EXTRA_LIBS)");
  } else if(str_start(compiler, "occ") || str_start(compiler, "orange")) {
    var_set("CC", "occ");
    var_set("LIB", "olib");
    var_set("LINK", "olink");
    exts.obj = ".o";
    exts.lib = ".l";
    var_push("CPPFLAGS", "-Dinline=__inline");
    // var_push("LDFLAGS", "/Wcm");
    var_push("CFLAGS", "-C+? +1 -v -E-36 -E-39");
    if(cfg.build_type == BUILD_TYPE_DEBUG || cfg.build_type == BUILD_TYPE_RELWITHDEBINFO) {
      var_push("CFLAGS", "+v");
      var_push("LDFLAGS", "-v -c+");
    }
    if(cfg.build_type == BUILD_TYPE_DEBUG)
      var_push("CFLAGS", "-O-");
    var_push("LDFLAGS", "-T:CON32");
    push_lib("DEFAULT_LIBS", "clwin");
    push_lib("DEFAULT_LIBS", "climp");
    // stralloc_copys(&commands.compile, "$(CC) /! /c $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(DEFS) -o$@ \"/I;\" $<");
    stralloc_copys(&commands.compile, "$(CC) /! /c $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(DEFS) -o$@ $<");
    set_command(&commands.lib, "$(LIB) /! $@", "$^");
    set_command(&commands.link, "$(LINK) -c /! $(LDFLAGS) $(EXTRA_LDFLAGS) -o$@", "$^ c0xpe.o $(LIBS) $(DEFAULT_LIBS)");
  } else if(str_start(compiler, "8cc")) {
    exts.lib = ".a";
    exts.obj = ".o";
    var_set("CC", "8cc");
  } else if(str_start(compiler, "dmc") || str_start(compiler, "digitalmars")) {
    // pathsep_args = '\\';
    var_set("CC", "dmc");
    var_set("LIB", "lib");
    var_set("CFLAGS", "");
    if(cfg.build_type == BUILD_TYPE_DEBUG || cfg.build_type == BUILD_TYPE_RELWITHDEBINFO) {
      var_push("CFLAGS", "-g");
      var_push("LDFLAGS", "-g");
    }
    if(cfg.build_type == BUILD_TYPE_MINSIZEREL) {
      var_push("CFLAGS", "-a1 -o+space ");
      var_push("LDFLAGS", "-Nc");
    } else if(cfg.build_type == BUILD_TYPE_DEBUG) {
      // var_push("CFLAGS", "-o-");
    } else {
      var_push("CFLAGS", "-o");
    }
    // set_command(&commands.lib, "$(LIB) -c $@", "$^");
    set_command(&commands.lib, "$(LIB) -c $@", "$^");
    stralloc_copys(&commands.compile, "$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(DEFS) -c -o$@ $<");
    set_command(&commands.link,
                "$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(LDFLAGS) $(EXTRA_LDFLAGS) -o$@",
                "$^ $(LIBS) $(EXTRA_LIBS) $(STDC_LIBS)");
  } else if(str_start(compiler, "pelles") || str_start(compiler, "po")) {
    var_set("CC", "cc");
    var_set("LINK", "polink");
    var_set("LIB", "polib");
    var_set("TARGET", cfg.mach.bits == _64 ? "amd64-coff" : "x86-coff");
    var_set("CFLAGS", "-W0");
    if(cfg.build_type != BUILD_TYPE_DEBUG)
      var_push("CFLAGS", "-Ob1");
    // var_push("CFLAGS", "-fp:precise");
    var_push("CFLAGS", "-Ze"); /* Activates Microsoft's
                                  extensions to C */
    // var_push("CFLAGS", "-Zx"); /*
    // Activates Pelle's extensions to C
    //*/
    var_push("CFLAGS", "-Go"); /* Accepts 'old' names
                                  for C runtime
                                  functions */
    // var_push("CFLAGS", "-Gz"); /*
    // default to __stdcall */
    var_push("CPPFLAGS", "-D__POCC__");
    if(cfg.mach.bits == _64) {
      var_set("MACHINE", "AMD64");
      var_set("L64", "64");
      // exts.lib = "64.lib";
      var_push("CPPFLAGS", "-D_M_AMD64");
    } else if(cfg.mach.bits == _32) {
      var_set("MACHINE", "X86");
      var_set("L64", "");
      var_push("CPPFLAGS", "-D_M_IX86");
    }
    var_push("CFLAGS", "-T$(TARGET)");
    var_push("LDFLAGS", "-machine:$(MACHINE)");
    var_push("LDFLAGS", "-libpath:\"%PELLESC%\\lib\"");
    var_push("LDFLAGS", "-libpath:\"%PELLESC%\\lib\\win$(L64)\"");
    /*    if(cfg.build_type ==
       BUILD_TYPE_MINSIZEREL)
          var_push("CFLAGS", "-Os");
        else*/
    if(cfg.build_type == BUILD_TYPE_DEBUG || cfg.build_type == BUILD_TYPE_RELWITHDEBINFO) {
      var_push("CFLAGS", "-Zi");
      var_push("LDFLAGS", "-DEBUG");
    }
    stralloc_copys(&commands.compile, "$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(DEFS) -c $< -Fo$@");
    stralloc_copys(&commands.link, "$(CC) $^ -Fe $@ $(LDFLAGS) $(EXTRA_LDFLAGS) $(LIBS) $(EXTRA_LIBS) $(STDC_LIBS)");
    pathsep_args = '\\';
    quote_args = "\"";
  } else if(str_start(compiler, "gp")) {
    var_set("AS", "gpasm");
    var_set("LINK", "gplink");
    var_set("LIB", "gplib");
    var_unset("CXX");
    cfg.mach.arch = PIC;
    exts.bin = ".cof";
    exts.obj = ".o";
    exts.lib = ".a";
    exts.src = ".asm";
    exts.inc = ".inc";

    if(cfg.build_type == BUILD_TYPE_DEBUG || cfg.build_type == BUILD_TYPE_RELWITHDEBINFO) {
      var_push("CFLAGS", "-g");
      var_push("LDFLAGS", "-g");
    }

    if(cfg.chip.len == 0)
      stralloc_copys(&cfg.chip, "16f876a");

    if(!var_isset("MACH")) {
      if(cfg.mach.bits == _14)
        var_set("MACH", "pic14");
      else
        var_set("MACH", "pic16");
    }
    if(cfg.mach.bits == _14) {
    }
    if(cfg.mach.bits == _16) {
    }
    if(cfg.build_type == BUILD_TYPE_MINSIZEREL) {
    } else if(cfg.build_type != BUILD_TYPE_DEBUG) {
    }
    if(cfg.build_type == BUILD_TYPE_DEBUG || cfg.build_type == BUILD_TYPE_RELWITHDEBINFO) {
      var_push("CFLAGS", "-d");
    }
    var_push("CFLAGS", "-p$(CHIP)");
    var_push("CPPFLAGS", "-D__$(CHIP)=1");
    var_push("CPPFLAGS", "-DGPUTILS=1");

    /*    if(cfg.mach.bits == _16) {
          var_push("LIBS", "-llibm18f.lib");
        } else {
          var_push("LIBS", "-llibm.lib");
        }*/
    set_command(&commands.lib, "$(LIB) rcs $@", "$^");
    stralloc_copys(&commands.compile, "$(AS) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(DEFS) -c $< -o $@");
    stralloc_copys(&commands.link, "$(LINK) $(LDFLAGS) $(EXTRA_LDFLAGS) -o $@ $^ $(LIBS) $(EXTRA_LIBS) $(STDC_LIBS)");
  } else if(str_start(compiler, "sdcc")) {
    var_set("CC", "sdcc");
    var_set("LINK", "sdcc");
    var_set("LIB", "sdar");
    var_unset("CXX");
    cfg.mach.arch = PIC;
    exts.bin = ".cof";
    exts.obj = ".o";
    exts.lib = ".a";
    // var_set("TARGET", cfg.mach.bits
    //== _14 ? "pic16" : "pic18");
    if(cfg.chip.len == 0)
      stralloc_copys(&cfg.chip, "16f876a");
    /* stralloc_nul(&cfg.chip);
     var_set("CHIP", cfg.chip.s);*/
    /* {
       stralloc chipdef;
       stralloc_init(&chipdef);
       stralloc_copys(&chipdef,
     "-DPIC"); stralloc_cat(&chipdef,
     &cfg.chip);
       stralloc_upper(&chipdef);
       stralloc_cats(&chipdef, "=1");
       var_push_sa("CPPFLAGS",
     &chipdef);
     }
    */
    if(!var_isset("MACH")) {
      if(cfg.mach.bits == _14)
        var_set("MACH", "pic14");
      else
        var_set("MACH", "pic16");
    }
    var_set("CFLAGS", "--use-non-free");
    if(cfg.mach.bits == _16)
      var_push("CFLAGS", "--pstack-model=large");
    if(cfg.mach.bits == _16) {
      var_push("CFLAGS", "--mplab-comp");
      // var_push("CFLAGS", "--extended");
      var_push("CFLAGS", "--pstack-model=large");
      var_push("CFLAGS", "--optimize-cmp");
      var_push("CFLAGS", "--optimize-df");
    }
    var_push("CFLAGS", "--float-reent");
    if(cfg.build_type == BUILD_TYPE_MINSIZEREL)
      var_push("CFLAGS", "--opt-code-size");
    else if(cfg.build_type != BUILD_TYPE_DEBUG)
      var_push("CFLAGS", "--opt-code-speed");
    if(cfg.build_type == BUILD_TYPE_DEBUG || cfg.build_type == BUILD_TYPE_RELWITHDEBINFO) {
      var_push("CFLAGS", "--debug");
      // var_push("LDFLAGS", "--debug");
    }
    var_push("CFLAGS", "-m$(MACH) -p$(CHIP)");
    var_push("CPPFLAGS", "-D__$(CHIP)=1");
    var_push("CPPFLAGS", "-DSDCC=1");
    // var_push("LDFLAGS", "--out-fmt-ihx");
    if(cfg.mach.bits == _16) {
      var_push("LIBS", "-llibm18f.lib");
    } else {
      var_push("LIBS", "-llibm.lib");
    }
    set_command(&commands.lib, "$(LIB) rcs $@", "$^");
    stralloc_copys(&commands.compile, "$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(DEFS) -c $< -o $@");
    stralloc_copys(
        &commands.link,
        "$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(LDFLAGS) $(EXTRA_LDFLAGS) -o $@ $^ $(LIBS) $(EXTRA_LIBS) $(STDC_LIBS)");
  } else if(str_start(compiler, "htc")) {
    var_unset("CXX");
    var_set("LIB", "libr");
    cfg.mach.arch = PIC;
    exts.bin = ".cof";
    exts.obj = ".p1";
    exts.lib = ".a";
    // var_set("CFLAGS", "--mode=pro");
    var_push("CFLAGS", "-N127");
    // var_push("CFLAGS", "-V");
    var_push("CPPFLAGS", "-DHI_TECH_C=1");
    var_set("TARGET", cfg.mach.bits == _14 ? "pic16" : "pic18");
    // var_push("CPPFLAGS", cfg.mach.bits == _14 ?
    //"-DPIC16=1" : "-DPIC18=1");
    var_set("CC", cfg.mach.bits == _14 ? "picc" : "picc18");
    var_set("LINK", cfg.mach.bits == _14 ? "picc" : "picc18");
    if(cfg.chip.len == 0)
      stralloc_copys(&cfg.chip, "16f876a");
    /*    stralloc_nul(&cfg.chip);
        var_set("CHIP", cfg.chip.s);
    */
    if(!var_isset("MACH")) {
      if(cfg.mach.bits == _14)
        var_set("MACH", "pic14");
      else
        var_set("MACH", "pic16");
    }
    if(cfg.build_type == BUILD_TYPE_MINSIZEREL)
      var_push("CFLAGS", "--opt=space");
    else if(cfg.build_type != BUILD_TYPE_DEBUG)
      var_push("CFLAGS", "--opt=speed");
    else
      var_push("CFLAGS", "--opt=all");
    if(cfg.build_type == BUILD_TYPE_DEBUG || cfg.build_type == BUILD_TYPE_RELWITHDEBINFO) {
      var_push("CFLAGS", "-g");
      var_push("CFLAGS", "--debugger=pickit3");
    }
    var_push("CFLAGS", "--double=32");
    // var_push("CFLAGS", "--warn=-3");
    var_push("CFLAGS", "-q");
    var_push("CFLAGS", "--chip=$(CHIP)");
    var_push("LDFLAGS", "--output=mcof");
    var_push("CFLAGS", "--runtime=default,-keep,+download");
    var_push("CFLAGS", "--summary=default");
    /*var_push("CFLAGS", "--errformat=\"%f:%l:%c error [%n]: %s\""); var_push("CFLAGS", "--warnformat=\"%f:%l:%c
     * warning
     * [%n]: %s\"");*/
    var_push("LDFLAGS", "--asmlist");
    // var_push("CPPFLAGS", "-D__$(CHIP)=1");
    set_command(&commands.lib, "$(LIB) $@", "$^");
    stralloc_copys(&commands.compile, "$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(DEFS) --pass1 -c $< -o$@");
    stralloc_copys(
        &commands.link,
        "$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(LDFLAGS) $(EXTRA_LDFLAGS) -o$@ $^ $(LIBS) $(EXTRA_LIBS) $(STDC_LIBS)");
  } else if(str_start(compiler, "xc8") || str_start(compiler, "picc")) {
    // no_libs = 1;
    var_unset("CXX");
    var_set("CC", "xc8");
    var_set("LINK", "mplink");
    var_set("LIB", "mplib");
    cfg.mach.arch = PIC;
    cmd_libs = 0;
    exts.bin = ".cof";
    exts.obj = ".p1";
    exts.lib = ".lpp";
    var_push("DEFS", "-D__XC=1");
    var_set("TARGET", cfg.mach.bits == _14 ? "pic16" : "pic18");
    var_push("CPPFLAGS", cfg.mach.bits == _14 ? "-DPIC16=1" : "-DPIC18=1");
    if(cfg.chip.len == 0)
      stralloc_copys(&cfg.chip, "16f876a");
    var_push("CFLAGS", "--mode=pro");
    var_push("CFLAGS", "--float=24");
    var_push("CFLAGS", "--double=32");
    if(cfg.build_type == BUILD_TYPE_MINSIZEREL)
      var_push("CFLAGS", "--opt=default,+asm,-asmfile,-speed,+space,+debug,3");
    else if(cfg.build_type != BUILD_TYPE_DEBUG)
      var_push("CFLAGS", "--opt=default,+asm,+asmfile,+speed,-space,-debug,9");
    else {
      var_push("CFLAGS", "--opt=default,+asm,+asmfile,-speed,-space,+debug");
      var_push("CFLAGS", "-g");
      var_push("CFLAGS", "--debugger=pickit3");
    }
    // var_push("CFLAGS", //"-fp:precise");
    var_push("CFLAGS", "-q");
    var_push("CFLAGS", "--asmlist");
    var_push("CFLAGS", "--chip=$(CHIP)");
    var_push("CPPFLAGS", "-D__$(CHIP)=1");
    var_push("LDFLAGS", "--output=-elf,+mcof");
    // var_push("LDFLAGS", "--output=-mcof,+elf:multilocs");
    var_push("LDFLAGS", "--summary=default");
    var_push("LDFLAGS", "--runtime=default,+clear,+init,-keep,-no_startup,-osccal,-resetbits,+download,+clib");
    // var_push("LDFLAGS", "--output=-default,elf,+mcof"); var_push("LDFLAGS", "--output=-mcof,+elf");
    var_push("LDFLAGS", "--stack=compiled");
    /*var_push("CFLAGS",
    "--errformat=\"%f:%l:%c error [%n]:
    %s\""); var_push("CFLAGS",
    "--warnformat=\"%f:%l:%c warning
    [%n]: %s\"");*/
    stralloc_copys(&commands.preprocess, "$(CPP) $(CPPFLAGS) $(DEFS) $< -o$@");
    stralloc_copys(&commands.compile,
                   "$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(DEFS) --pass1 -c $< -o$@");
    stralloc_copys(
        &commands.link,
        "$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(LDFLAGS) $(EXTRA_LDFLAGS) -o$@ $^ $(LIBS) $(EXTRA_LIBS) $(STDC_LIBS)");
    stralloc_copys(&commands.lib,
                   "$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(LDFLAGS) $(EXTRA_LDFLAGS) --OUTPUT=lpp --memorysummary -G "
                   "-m$@.map -P --asmlist --output=default,-inhx032 --output=-mcof,+elf:multilocs -o$@.elf $^ $(LIBS) "
                   "$(EXTRA_LIBS) $(STDC_LIBS)");
  } else {
    return 0;
  }
  if(cfg.build_type == BUILD_TYPE_DEBUG) {
    var_push("DEFS", "-D_DEBUG=1");
  } else {
    var_push("DEFS", "-DNDEBUG=1");
    if(str_equal(exts.src, ".c"))
      var_push("CFLAGS", cfg.build_type == BUILD_TYPE_MINSIZEREL ? "-O1" : "-O2");
  }
  if(cfg.mach.arch == PIC) {
    stralloc chipdef;
    stralloc_init(&chipdef);
    stralloc_cat(&chipdef, &cfg.chip);
    stralloc_lower(&chipdef);
    stralloc_cats(&chipdef, "=1");
    stralloc_inserts(&chipdef, "-D__", 0);
    var_push_sa("CPPFLAGS", &chipdef);
  }
  if(cfg.sys.os == OS_WIN) {
    // push_lib("EXTRA_LIBS", "advapi32");
    /*  if(str_start(compiler, "dmc"))
        push_lib("EXTRA_LIBS", "wsock32"); else
        push_lib("EXTRA_LIBS", "ws2_32");
    */
    push_lib("EXTRA_LIBS", "kernel32");
  }
  if(cygming) {
    if(!ninja)
      pathsep_args = '/';
    var_set("prefix", "/");
    var_push("prefix", str_start(tools.toolchain, "mingw") ? tools.toolchain : "usr");
    if(cygming && 0) {
      var_t* cross = var_set("CROSS_COMPILE", str_end(tools.toolchain, "64") ? "x86_64" : "i686");
      // cross->sep = '-';
      stralloc_cats(&cross->value.sa, str_start(tools.toolchain, "mingw") ? "-w64-" : "-pc-");
      stralloc_cats(&cross->value.sa,
                    str_start(tools.toolchain, "mingw")  ? "mingw32"
                    : str_start(tools.toolchain, "msys") ? "msys"
                                                         : "cygwin");
      stralloc_catc(&cross->value.sa, '-');
    }
  }
  if(var_isset("CROSS_COMPILE")) {
    stralloc_prepends(&commands.compile, "$(CROSS_COMPILE)");
    stralloc_prepends(&commands.lib, "$(CROSS_COMPILE)");
    stralloc_prepends(&commands.link, "$(CROSS_COMPILE)");
  }
  return 1;
}

/**
 * @brief usage  Show command line usage
 * @param errmsg_argv0
 */
void
usage(char* errmsg_argv0) {
  buffer_putm_internal(buffer_1,
                       "Usage: ",
                       str_basename(errmsg_argv0),
                       " [sources...]\n"
                       "\n"
                       "Options\n"
                       "  -h, --help                show this help\n"
                       "\n"
                       "  -o, --output FILE         write to file\n"
                       "  -f, --input-file FILE     read from input file\n"
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
                       "  -S, --build-as-lib TARGET build target as library\n"
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
                       "     zapcc[-cl]  ZapCC\n"
                       "     htc         Hi-Tech C for PIC or PIC18\n"
                       "     xc8         Microchip C Compiler for 8-bit PIC\n"
                       "     sdcc        Small Device C Compiler\n"
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
                       "     mplab       MPLAB project (.mcp)\n"
                       "     mplabx      MPLAB X project (NetBeans)\n"
                       "     cmake       KitWare CMake\n"
                       "\n"
                       "  Specify build type:\n\n"
                       "    --debug            with debugging information, no optimization\n"
                       "    --relwithdebinfo   with debugging information, optimize for speed\n"
                       "    --release          optimize for speed\n"
                       "    --minsizerel       optimize for size\n"
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
  int c;
  int ret = 0, index = 0;
  const char* dir = NULL;
  set_t toks;
  strarray args;
  strlist cmdline;
  static strarray libs, includes;
  buffer filebuf;
  buffer* out = buffer_1;
  char** it;
  const char* s;
  size_t n;
  target *all = 0, *compile = 0;
  char** arg;
  char **ptr, *x;
  strarray sources;
  strarray_init(&sources);

#if !WINDOWS_NATIVE
  sig_ignore(SIGTRAP);
#endif

  struct unix_longopt opts[] = {
      {"help", 0, NULL, 'h'},
      {"objext", 1, NULL, 'O'},
      {"exeext", 1, NULL, 'B'},
      {"libext", 1, NULL, 'X'},
      {"create-libs", 0, &cmd_libs, 1},
      {"create-objs", 0, &cmd_objs, 1},
      {"create-bins", 0, &cmd_bins, 1},
      {"no-create-libs", 0, &cmd_libs, 0},
      {"no-create-objs", 0, &cmd_objs, 0},
      {"no-create-bins", 0, &cmd_bins, 0},
      {"name", 0, 0, 'n'},
      {"install", 0, 0, 'i'},
      {"includedir", 0, 0, 'I'},
      /*                           {"install-bins",
         0, &inst_bins, 1},
                                {"install-libs",
         0, &inst_libs, 1},*/
      {"builddir", 1, 0, 'd'},
      {"workdir", 1, 0, 'w'},
      {"compiler-type", 1, 0, 't'},
      {"make-type", 1, 0, 'm'},
      {"arch", 1, 0, 'a'},
      {"system", 1, 0, 's'},
      {"release", 0, &cfg.build_type, BUILD_TYPE_RELEASE},
      {"Release", 0, &cfg.build_type, BUILD_TYPE_RELEASE},
      {"relwithdebinfo", 0, &cfg.build_type, BUILD_TYPE_RELWITHDEBINFO},
      {"RelWithDebInfo", 0, &cfg.build_type, BUILD_TYPE_RELWITHDEBINFO},
      {"minsizerel", 0, &cfg.build_type, BUILD_TYPE_MINSIZEREL},
      {"MinSizeRel", 0, &cfg.build_type, BUILD_TYPE_MINSIZEREL},
      {"debug", 0, &cfg.build_type, BUILD_TYPE_DEBUG},
      {"Debug", 0, &cfg.build_type, BUILD_TYPE_DEBUG},
      {"define", 1, NULL, 'D'},
      {"build-as-lib", 0, 0, 'S'},
      {"input-file", 0, 0, 'f'},
      {"cross", 0, 0, 'c'},
      {"chip", 1, 0, 'p'},
      {"preprocessor", 1, 0, 'P'},
      {"lang-c", 0, &cfg.lang, LANG_C},
      {"cxx", 0, &cfg.lang, LANG_CXX},
      {"c++", 0, &cfg.lang, LANG_CXX},
      {0, 0, 0, 0},
  };
  errmsg_iam(argv[0]);
  uint32_seed(NULL, 0);

#ifdef _MSC_VER
  unix_optbuf = buffer_1;

#endif

  byte_zero(&cfg, sizeof(cfg));
  byte_zero(&dirs, sizeof(dirs));
  byte_zero(&sourcelist, sizeof(sourcelist));
  byte_zero(&rules, sizeof(rules));
  byte_zero(&vars, sizeof(vars));
  byte_zero(&srcs, sizeof(srcs));
  byte_zero(&bins, sizeof(bins));
  byte_zero(&tools, sizeof(tools));
  set_init(&link_libraries, 0);
  set_init(&build_directories, 0);
  MAP_NEW(sourcedirs);
  MAP_NEW(targetdirs);
  MAP_NEW(rules);
  MAP_NEW(vars);

#if defined(_WIN32) || defined(_WIN64) || WINDOWS_NATIVE
  cfg.sys.os = OS_WIN;
  cfg.sys.type = NTOS;
#elif defined(__linux__) || defined(__unix__)
  cfg.sys.os = OS_LINUX;
  cfg.sys.type = UNIX;
#elif defined(__APPLE__)
  cfg.sys.os = OS_MAC;
  cfg.sys.type = UNIX;
#endif

  pathsep_args = WINDOWS_NATIVE ? '\\' : '/';
  pathsep_make = cfg.sys.type == NTOS ? '\\' : '/';
  if((s = env_get("PATH")) == 0)
    s = "/usr/local/bin:/usr/bin:/bin";
  path_split(s, &system_path);
  strlist_init(&vpath, ' ');
  strlist_init(&cmdline, ' ');
  strlist_fromv(&cmdline, (const char**)argv, argc);

  strlist_init(&dirs.this, pathsep_make);
  strlist_init(&dirs.out, pathsep_make);
  strlist_init(&dirs.build, pathsep_make);
  strlist_init(&dirs.work, pathsep_make);
  exts.src = ".c";
  exts.inc = ".h";

  for(;;) {
    const char* arg;
    c = unix_getopt_long(argc, argv, "habo:O:B:L:d:t:m:n:a:D:l:I:c:s:p:P:S:if:Cw:", opts, &index);
    if(c == -1)
      break;
    if(c == 0)
      continue;
    arg = unix_optarg ? unix_optarg : argv[unix_optind];

    switch(c) {
      case 'h':
        usage(argv[0]);
        ret = 0;
        goto quit;
      case 'C': {
        cfg.lang = LANG_C;
        break;
      }
      case 'c': {
        cross_compile = arg;
        break;
      }
      case 'b': {
        cmd_bins = 1;
        cmd_objs = 1;
        break;
      }
      case 'o': {
        outfile = arg;
        break;
      }
      case 'O': {
        exts.obj = arg;
        break;
      }
      case 'B': {
        exts.bin = arg;
        break;
      }
      case 'S': {
        strlist_push(&build_as_lib, arg);
        break;
      }
      case 'X': {
        exts.lib = arg;
        break;
      }
      case 'd': {
        dir = arg;
        break;
      }
      case 'w': {
        stralloc_copys(&dirs.work.sa, arg);
        break;
      }
      case 't': {
        tools.toolchain = tools.compiler = arg;
        break;
      }
      case 'm': {
        tools.make = arg;
        break;
      }
      case 'P': {
        tools.preproc = arg;
        break;
      }
      case 'a': {
        set_machine(arg);
        break;
      }
      case 's': {
        set_system(arg);
        break;
      }
      case 'n': {
        stralloc_copys(&output_name, arg);
        break;
      }
      case 'p': {
        if(arg)
          set_chip(arg);
        break;
      }
      case 'f': {
        infile = arg;
        break;
      }
      case 'l': {
        strarray_push(&libs, arg);
        break;
      }
      case 'i': {
        inst_bins = 1;
        inst_libs = 1;
        break;
      }
      case 'D': {
        push_define(arg);
        break;
      }
      case 'I': {
        buffer_puts(buffer_2, "Add -I: ");
        buffer_puts(buffer_2, arg);
        buffer_putnlflush(buffer_2);
        strarray_push(&includes, arg);
        break;
      }
      default:
        buffer_puts(buffer_2, "No such option '");
        buffer_putlong(buffer_2, c);
        buffer_putsflush(buffer_2, "'\n");
        // usage(argv[0]);
        ret = 1;
        goto quit;
    }
  }
  if(!cmd_bins && !cmd_libs && !cmd_objs) {
    cmd_bins = 1;
    cmd_objs = 1;
    cmd_libs = 1;
  }
  if(inst_bins)
    cmd_bins = 1;
  if(!cmd_libs)
    inst_libs = 0;
  if(!format_linklib_fn)
    format_linklib_fn = &format_linklib_lib;

  path_getcwd(&dirs.this.sa);
  if(cfg.build_type == -1) {
    if((cfg.build_type = extract_build_type(&dirs.build.sa)) == -1)
      if((cfg.build_type = extract_build_type(&dirs.this.sa)) == -1)
        cfg.build_type = extract_build_type(&dirs.out.sa);
  }
  if(cfg.build_type == -1)
    cfg.build_type = BUILD_TYPE_DEBUG;
  if(tools.make == NULL && tools.compiler) {
    if(str_start(tools.compiler, "b")) {
      tools.make = "borland";

    } else if(str_start(tools.compiler, "msvc")) {
      tools.make = "nmake";
    } else if(str_start(tools.compiler, "g")) {
      tools.make = "gmake";
      tool_config |= MAKE_PATTERN_RULES;
    } else if(str_start(tools.compiler, "o")) {
      tools.make = "omake";
    } else if(str_start(tools.compiler, "po")) {
      tools.make = "pomake";
    }
  }
  if(tools.toolchain)
    cygming =
        str_start(tools.toolchain, "mingw") || str_start(tools.toolchain, "cyg") || str_start(tools.toolchain, "msys");
  if(cygming) {
    tools.compiler = "gcc";
    if(tools.make == 0)
      tools.make = "gmake";
  }
  if(tools.make == NULL)
    tools.make = "make";
  batch = str_start(tools.make, "bat") || str_start(tools.make, "cmd");
  ninja = tools.make[str_find(tools.make, "ninja")] != '\0';
  shell = str_start(tools.make, "sh");
  if(batch)
    comment = "REM ";
  if(tools.compiler == NULL) {
    if(cfg.mach.arch == PIC)
      tools.compiler = "xc8";
    if(tools.compiler == NULL)
      tools.compiler = "gcc";
    else if(cfg.mach.bits == 0)
      set_machine(tools.compiler);
  }
  // debug_sa("dirs.build"", &dirs.build.sa);
  set_init(&toks, 0);
  {
    strlist tmp;
    strlist_init(&tmp, '\0');
    stralloc_copy(&tmp.sa, &dirs.build.sa);
    if(outfile)
      strlist_push(&tmp, outfile);
    stralloc_replacec(&tmp.sa, '/', '\0');
    stralloc_replacec(&tmp.sa, '-', '\0');
    stralloc_replacec(&tmp.sa, '.', '\0');
    strlist_foreach(&tmp, s, n) { set_add(&toks, s, n); }

#ifdef DEBUG_OUTPUT_
    buffer_puts(buffer_2, "toks: ");
    buffer_putset(buffer_2, &toks, " ", 1);
#endif
  }
  {
    size_t i;
    set_iterator_t it;
    stralloc tok;
    stralloc_init(&tok);
    set_foreach(&toks, it, s, n) {
      stralloc_copyb(&tok, s, n);
      stralloc_nul(&tok);
      if(set_compiler_type(tok.s)) {
        tools.compiler = (char*)s;
        break;
      }
      if(cfg.build_type == -1) {
        for(i = 0; i < (sizeof(build_types) / sizeof(build_types[0])); ++i) {
          if(s[case_find(s, build_types[i])]) {
            cfg.build_type = i;
            break;
          }
        }
      }
    }
    stralloc_free(&tok);
  }
  if(!set_make_type() || !set_compiler_type(tools.compiler)) {
    usage(argv[0]);
    ret = 2;
    goto quit;
  }
  if(*cross_compile) {
    var_set("CROSS_COMPILE", cross_compile);
    if(var_isset("CC"))
      stralloc_prepends(&var_list("CC", pathsep_args)->value.sa, "$(CROSS_COMPILE)");
    if(var_isset("CXX"))
      stralloc_prepends(&var_list("CXX", pathsep_args)->value.sa, "$(CROSS_COMPILE)");
    if(var_isset("AR"))
      stralloc_prepends(&var_list("AR", pathsep_args)->value.sa, "$(CROSS_COMPILE)");
  }
  batchmode = batch && stralloc_contains(&commands.compile, "-Fo");
  if(batch)
    pathsep_args = pathsep_make;
  strarray_foreach(&libs, it) { with_lib(*it); }
  strarray_foreach(&includes, it) { includes_add(*it); }
  stralloc_replacec(&dirs.out.sa, PATHSEP_C == '/' ? '\\' : '/', PATHSEP_C);
  // path_absolute_sa(&dirs.out.sa);
  strlist_nul(&dirs.out);
  strlist_nul(&dirs.this);

  if(outfile)
    path_dirname(outfile, &dirs.work.sa);
  else
    stralloc_copys(&dirs.work.sa, "build");

  path_concat_sa(&dirs.this.sa, &dirs.work.sa, &dirs.out.sa);

  if(dirs.build.sa.len == 0) {
    if(strlist_contains(&dirs.work, "build") && strlist_count(&dirs.work) > 1) {
      stralloc_copy(&dirs.build.sa, &dirs.work.sa);
      // path_relative_to(dirs.out.sa.s, dirs.this.sa.s, &dirs.build.sa);
    } else if(tools.toolchain && !strlist_contains(&dirs.this, "build")) {
      stralloc target;
      stralloc_init(&target);
      stralloc_copys(&target, tools.toolchain);
      if(cfg.chip.s) {
        stralloc_cats(&target, "-");
        stralloc_cat(&target, &cfg.chip);
      }
      stralloc_nul(&target);
      stralloc_copy(&dirs.build.sa, &dirs.work.sa);
      // strlist_push(&dirs.build, dir ? dir : "build");
      if(cross_compile && *cross_compile) {
        strlist_push(&dirs.build, cross_compile);
      } else {
        strlist_push_sa(&dirs.build, &target);
        stralloc_catc(&dirs.build.sa, '-');
        stralloc_cats(&dirs.build.sa, build_types[cfg.build_type]);
      }
      stralloc_free(&target);
    }
    stralloc_replacec(&dirs.build.sa, PATHSEP_C == '/' ? '\\' : '/', PATHSEP_C);
  }
  if(dirs.work.sa.len == 0)
    stralloc_copys(&dirs.work.sa, ".");
  path_absolute_sa(&dirs.out.sa);
  path_canonical_sa(&dirs.out.sa);
  path_absolute_sa(&dirs.build.sa);
  path_canonical_sa(&dirs.build.sa);
  strlist_nul(&dirs.this);
  strlist_nul(&dirs.out);
  strlist_nul(&dirs.build);
  strlist_nul(&dirs.work);
  // debug_sa("dirs.this", &dirs.this.sa);
  // debug_sa("dirs.out", &dirs.out.sa);
  // debug_sa("dirs.build", &dirs.build.sa);
  if(tools.preproc)
    var_set("CPP", tools.preproc);
  includes_cppflags();

  // debug_sa("dirs.work", &dirs.work.sa);
  strlist_nul(&dirs.this);
  strlist_nul(&dirs.out);
  path_relative_to(dirs.this.sa.s, dirs.out.sa.s, &srcdir);
  stralloc_nul(&srcdir);
  // debug_sa("srcdir", &srcdir);
  if(dirs.out.sa.len) {
    stralloc_replacec(&dirs.this.sa, PATHSEP_C == '/' ? '\\' : '/', PATHSEP_C);
    stralloc_replacec(&dirs.out.sa, PATHSEP_C == '/' ? '\\' : '/', PATHSEP_C);
    // debug_sa("dirs.this", &dirs.this.sa);
    // debug_sa("dirs.out", &dirs.out.sa);
    path_absolute_sa(&dirs.out.sa);
    stralloc_zero(&tmp);
    path_relative_to(dirs.this.sa.s, dirs.out.sa.s, &tmp);
    // if(tmp.len) {
    stralloc_copy(&srcdir, &tmp);
    // debug_sa("srcdir", &srcdir);
    //}
    stralloc_zero(&tmp);
  }
  // debug_sa("srcdir", &srcdir);
  path_relative_to(dirs.build.sa.s, dirs.out.sa.s, &tmp);
  // debug_sa("tmp", &tmp);
  /* if(dirs.build.sa.len > dirs.out.sa.len && byte_equal(dirs.out.sa.s, dirs.out.sa.len, dirs.build.sa.s)) {
     const char* x = dirs.build.sa.s + dirs.out.sa.len;
     size_t n = dirs.build.sa.len - dirs.out.sa.len;
     stralloc_zero(&dirs.work.sa);
     if(n > 0 && *x == '/') {
       x++;
       n--;
     }
     stralloc_catb(&dirs.work.sa, x, n);
   }*/
  // path_relative_to(dirs.build.sa.s,  dirs.out.sa.s, &dirs.work.sa);
  strlist_nul(&dirs.work);
  stralloc_replacec(&dirs.work.sa, pathsep_make == '/' ? '\\' : '/', pathsep_make);

  /*  if(outfile)
      path_dirname(outfile, &dirs.out.sa);*/

  mkdir_components(&dirs.out, 0755);
  if(stralloc_diffs(&dirs.work.sa, "."))
    mkdir_components(&dirs.work, 0755);
  mkdir_components(&dirs.build, 0755);

  if(outfile) {
    if('\\' != PATHSEP_C)
      stralloc_replacec(&dirs.out.sa, '\\', PATHSEP_C);
    if(stralloc_equals(&dirs.out.sa, "."))
      stralloc_zero(&dirs.out.sa);
    else
      stralloc_catc(&dirs.out.sa, pathsep_make);
    byte_zero(&filebuf, sizeof(filebuf));
    if(buffer_truncfile(&filebuf, outfile)) {
      errmsg_warnsys("ERROR: opening '", outfile, "'", 0);
      ret = 2;
      goto quit;
    }
    out = &filebuf;
    // path_absolute_sa(&dirs.out.sa);
  }

  /*
    if(tmp.len) {
      stralloc_catc(&tmp, pathsep_make);
      stralloc_copy(&dirs.build.sa,
    &tmp);
    }
    stralloc_free(&tmp);
    //debug_sa("dirs.build",
    &dirs.build.sa);
  */
  all = rule_get("all");
  all->phony = true;

  stralloc_catc(&all->recipe, '\n');

  if(tool_config & (MAKE_PATTERN_RULES | MAKE_IMPLICIT_RULES)) {
    stralloc rn;
    bool outputs = false;
    stralloc_init(&rn);
    if(tool_config & MAKE_PATTERN_RULES) {
      stralloc_copys(&rn, "$(BUILDDIR)");
      stralloc_cats(&rn, "%");
      stralloc_cats(&rn, exts.obj);
      stralloc_cats(&rn, ": %");
      stralloc_cats(&rn, exts.src);
      outputs = true;
    } else {
      stralloc_copys(&rn, exts.src);
      stralloc_cats(&rn, exts.obj);
    }
    compile = rule_get_sa(&rn);
    compile->outputs = outputs;

    stralloc_free(&rn);
  }

  if(str_equal(exts.src, ".asm")) {
    target* assemble = rule_get(".asm.o");

    stralloc_copy(&assemble->recipe, &commands.compile);
  }

  strarray_init(&args);
  strarray_init(&srcs);

  if(infile) {
    input_process(infile, all);

#ifdef DEBUG_OUTPUT_
    buffer_puts(buffer_2, "build_directories =\n\t");
    buffer_putset(buffer_2, &build_directories, "\n\t", 2);
    buffer_putnlflush(buffer_2);
#endif
    // stralloc_free(&dirs.work.sa);
    {
      stralloc builddir;
      stralloc_init(&builddir);

      set_at_sa(&build_directories, 0, &builddir);

      var_setb(builddir_varname, builddir.s, builddir.len);
      stralloc_free(&builddir);
    }
  }
  if(compile)
    stralloc_weak(&compile->recipe, &commands.compile);

  if(compile) {
    strlist_nul(&dirs.work);
    strlist_push_unique(&vpath, ".");
    strlist_push_unique_sa(&vpath, &dirs.work.sa);

    set_clear(&compile->output);
    MAP_PAIR_T it;

    MAP_FOREACH(rules, it) {
      target* rule = MAP_ITER_VALUE(it);

      if(rule_is_compile(rule) && rule != compile) {
        stralloc_free(&rule->recipe);
        stralloc_init(&rule->recipe);

        if(str_equal(tools.make, "gmake"))
          rule->disabled = 1;

        // set_cat(&compile->prereq, &rule->prereq);
        set_cat(&compile->output, &rule->output);
      }
    }
  }
  while(unix_optind < argc) {
    stralloc arg;
    stralloc_init(&arg);
    stralloc_copys(&arg, argv[unix_optind]);
    stralloc_nul(&arg);
    if(stralloc_contains(&arg, "=")) {
      size_t eqpos;
      const char* v;
      // debug_sa("Setting var", &arg);
      eqpos = str_chr(arg.s, '=');
      arg.s[eqpos++] = '\0';
      v = &arg.s[eqpos];
      var_set(arg.s, v);
      ++unix_optind;
      continue;
    }
    stralloc_replacec(&arg, PATHSEP_C == '/' ? '\\' : '/', PATHSEP_C);
    stralloc_nul(&arg);

#if WINDOWS_NATIVE || defined(__MINGW32__)
    if(str_rchrs(argv[unix_optind], "*?", 2) < str_len(argv[unix_optind]))
      strarray_glob(&args, arg.s);
    else
#endif

      strarray_push(&args, arg.s);
    ++unix_optind;
  }
  path_canonical_sa(&dirs.out.sa);
  path_collapse_sa(&dirs.out.sa);

#ifdef DEBUG_OUTPUT
  debug_sa("dirs.work: ", &dirs.work.sa);
  debug_sa("dirs.build: ", &dirs.build.sa);
  debug_sa("dirs.out: ", &dirs.out.sa);
  debug_sa("dirs.this: ", &dirs.this.sa);
  buffer_putnlflush(buffer_2);
#endif

  /* No arguments given */
  if(strarray_size(&args) == 0 && !infile) {
    buffer_putsflush(buffer_2, "ERROR: No arguments given\n\n");
    usage(argv[0]);
    ret = 1;
    goto quit;
  }
  gen_mkdir_rule(&dirs.work.sa);
  add_path_sa(&all->prereq, &dirs.work.sa);

#ifdef DEBUG_OUTPUT
  {
    size_t n;

    buffer_puts(buffer_2, "dirs.out[]: ");
    buffer_putulong(buffer_2, strlist_count(&dirs.out));
    buffer_puts(buffer_2, ", dirs.out[0]: ");
    buffer_put(buffer_2, strlist_at_n(&dirs.out, 0, &n), n);
    buffer_putnlflush(buffer_2);
  }
#endif

  if(!stralloc_equals(&dirs.out.sa, "./")) {
    gen_mkdir_rule(&dirs.out.sa);
    add_path_sa(&all->prereq, &dirs.out.sa);
  }

  strarray_dump(buffer_2, &args);
  strarray_foreach(&args, arg) {

#ifdef DEBUG_OUTPUT_
    buffer_puts(buffer_2, "argument: ");
    buffer_puts(buffer_2, *arg);
    buffer_putnlflush(buffer_2);
#endif
#if 0 // WINDOWS_NATIVE
    glob_t gl;
    size_t i;
    if(glob(*arg, GLOB_TILDE | GLOB_BRACE, 0, &gl)) {
      buffer_putm_internal(buffer_2, "ERROR: glob() ", *arg, newline, NULL);
      buffer_flush(buffer_2);
      continue;
    }
    for(i = 0; i < gl.gl_matchc; i++)
#else

#endif

    {

#if 0 // WINDOWS_NATIVE
      const char* p = gl.gl_pathv[i];
#else
      const char* p = *arg;
#endif

      if(!path_exists(p)) {
        buffer_putm_internal(buffer_2, "ERROR: Doesn't exist: ", p, newline, NULL);
        buffer_flush(buffer_2);
        ret = 127;
        goto fail;
      }
      if(is_source(p) || is_include(p))
        sources_add(p, &dirs.this.sa);
      else if(path_is_directory(p))
        sources_get(p, &dirs.this.sa);
    }
  }

  {
    set_iterator_t it;
    set_foreach(&srcs, it, x, n) {

#ifdef DEBUG_OUTPUT_
      buffer_puts(buffer_2, "adding to sources:");
      buffer_put(buffer_2, x, n);
      buffer_putnlflush(buffer_2);
#endif
      if(is_source_b(x, n))
        strarray_pushb(&sources, x, n);
    }
    strarray_sort(&sources, &sources_sort);
  }

#ifdef DEBUG_OUTPUT_
  buffer_puts(buffer_2, "strarray sources:");
  strarray_dump(buffer_2, &sources);
  buffer_putnlflush(buffer_2);
#endif

  if(str_start(tools.make, "g")) {
    stralloc builddir;
    stralloc_init(&builddir);
    path_relative_to(dirs.build.sa.s, dirs.work.sa.s, &builddir);
    stralloc_nul(&builddir);
    if(!stralloc_endc(&dirs.work.sa, PATHSEP_C))
      stralloc_catc(&dirs.work.sa, PATHSEP_C);
    var_set(builddir_varname, dirs.work.sa.s);
    stralloc_copys(&dirs.build.sa, "$(BUILDDIR)");
  }

  if(((batch | shell) && stralloc_equals(&dirs.work.sa, ".")))
    batchmode = 1;
  if(output_name.len) {
    project_name = str_ndup(output_name.s, output_name.len);
  } else {
    stralloc abspath;
    stralloc_init(&abspath);
    path_absolute(dirs.this.sa.s, &abspath);
    stralloc_nul(&abspath);
    project_name = str_dup(path_basename(abspath.s));
    stralloc_free(&abspath);
  }

  if(!infile) {
    stralloc src;
    stralloc_init(&src);
    strarray_foreach(&sources, ptr) {

#ifdef DEBUG_OUTPUT_
      buffer_putm_internal(buffer_2, PINK256 "sourcedir_addsource" NC "(\"", *ptr, "\")", NULL);
      buffer_putnlflush(buffer_2);
#endif

      sourcedir_addsource(*ptr, &sources, exts.bin, exts.src, &dirs.this.sa, &dirs.out.sa, &progs, &bins, pathsep_make);
    }
    sourcedir_populate(&sources);
    strarray_free(&sources);
    stralloc_free(&src);

#ifdef DEBUG_OUTPUT_
    buffer_puts(buffer_2, "targetdirs:\n");
    MAP_FOREACH(targetdirs, t) {
      uint32* count_ptr = (uint32*)MAP_ITER_VALUE(t);
      buffer_puts(buffer_2, "  '");
      buffer_puts(buffer_2, MAP_ITER_KEY(t));
      buffer_puts(buffer_2, "' => ");
      buffer_putulong(buffer_2, *count_ptr);
      buffer_putnlflush(buffer_2);
    }
    buffer_putnlflush(buffer_2);
#endif

#ifdef DEBUG_OUTPUT_
    buffer_puts(buffer_2, "cmd_libs = ");
    buffer_putlong(buffer_2, cmd_libs);
    buffer_puts(buffer_2, " cmd_bins = ");
    buffer_putlong(buffer_2, cmd_libs);
    buffer_puts(buffer_2, " cmd_objs = ");
    buffer_putlong(buffer_2, cmd_objs);
    buffer_putnlflush(buffer_2);
#endif

#ifdef DEBUG_OUTPUT_
    MAP_FOREACH(targetdirs, t) {
      uint32* count_ptr = (uint32*)MAP_ITER_VALUE(t);
      buffer_puts(buffer_2, "  '");
      buffer_puts(buffer_2, MAP_ITER_KEY(t));
      buffer_puts(buffer_2, "' => ");
      buffer_putulong(buffer_2, *count_ptr);
      buffer_putnlflush(buffer_2);
    }
    buffer_putnlflush(buffer_2);
    sourcedir_dump_all(buffer_2, sourcedirs);
#endif

    if(cmd_libs) {
      gen_lib_rules();
      deps_for_libs();
    } else {
      MAP_PAIR_T t;
      MAP_FOREACH(sourcedirs, t) {
        sourcedir* srcdir = *(sourcedir**)MAP_ITER_VALUE(t);
        /*if(tools.preproc) {
          gen_simple_compile_rules(rules, srcdir, MAP_ITER_KEY(t), exts.src, exts.pps, &commands.preprocess);
          gen_simple_compile_rules(rules, srcdir, MAP_ITER_KEY(t), exts.pps, exts.obj, &commands.compile); }
        else */
        { gen_simple_compile_rules(srcdir, MAP_ITER_KEY(t), exts.src, exts.obj, &commands.compile); }
      }
    }
    if(cmd_bins) {

#ifdef DEBUG_OUTPUT_
      buffer_puts(buffer_2, "sourcelist.length = ");
      buffer_putulong(buffer_2, dlist_length(&sourcelist));
      buffer_putnlflush(buffer_2);
#endif

      cmd_bins = gen_link_rules();

#ifdef DEBUG_OUTPUT_
      buffer_puts(buffer_2, "bins = ");
      buffer_putstra(buffer_2, &bins, ", ");
      buffer_putnlflush(buffer_2);
#endif

#ifdef DEBUG_OUTPUT_
      buffer_puts(buffer_2, "progs = ");
      buffer_putstra(buffer_2, &progs, ", ");
      buffer_putnlflush(buffer_2);
#endif
    }
    if(cmd_bins == 0 || cmd_libs == 1) {
      MAP_PAIR_T t;
      MAP_FOREACH(rules, t) {
        target* tgt = MAP_ITER_VALUE(t);
        if(stralloc_equal(&tgt->recipe, &commands.lib) && cmd_libs)
          set_adds(&all->prereq, MAP_ITER_KEY(t));
      }
    }
  }

  gen_clean_rule();

  {
    MAP_PAIR_T t;
    MAP_FOREACH(rules, t) {
      target* rule = MAP_ITER_VALUE(t);

      if(rule_is_link(rule))
        set_adds(&all->prereq, rule->name);
    }
  }

  if(inst_bins || inst_libs)
    gen_install_rules();
  {
    MAP_PAIR_T t;
    MAP_FOREACH(sourcedirs, t) {
      sourcedir* srcdir = *(sourcedir**)MAP_ITER_VALUE(t);

#if DEBUG_OUTPUT_
      buffer_putm_internal(buffer_2, "key: ", t->key, " pptoks: ", NULL);
      buffer_putset(buffer_2, &srcdir->pptoks, ", ", 2);
#endif
    }
  }

#ifdef DEBUG_OUTPUT
  buffer_puts(buffer_2, "Dumping all rules...\n");
  {
    int i = 0;
    MAP_PAIR_T t;
    MAP_FOREACH(rules, t) {
      target* rule = MAP_ITER_VALUE(t);
      buffer_puts(buffer_2, PINK256 "Rule" NC " #");
      buffer_putlong(buffer_2, ++i);
      rule_dump(rule);
      buffer_putnlflush(buffer_2);
    }
  }
#endif
fail:
  if(!case_diffs(tools.make, "mplab")) {
    output_mplab_project(out, 0, 0, &include_dirs);
    goto quit;
  }

#ifdef DEBUG_OUTPUT
  {
    strlist rule_names;
    strlist_init(&rule_names, '\0');

    map_keys(&rules, &rule_names);

    buffer_puts(buffer_2, "rule_names:\n\t");
    buffer_putsl(buffer_2, &rule_names, "\n\t");
    buffer_putnlflush(buffer_2);
    strlist_free(&rule_names);
  }
#endif

  if(!case_diffs(tools.make, "cmake")) {
    output_cmake_project(out, &rules, &vars, &include_dirs, &link_dirs);
    goto quit;
  }

  buffer_putm_internal(out, comment, " Generated by:", newline, comment, "  ", NULL);
  buffer_putsa(out, &cmdline.sa);
  buffer_putsflush(out, newline);
  stralloc_nul(&cfg.chip);
  var_set("CHIP", cfg.chip.s);

  if(ninja) {
    stralloc tmp;
    stralloc_init(&tmp);
    path_relative_to_sa(&dirs.build.sa, &dirs.out.sa, &tmp);

    while(stralloc_endb(&tmp, &pathsep_args, 1))
      tmp.len--;

    var_setb("objdir", tmp.s, tmp.len);
    var_set("extra_cflags", "$$EXTRA_CFLAGS");
    var_set("extra_ldflags", "$$EXTRA_LDFLAGS");
    stralloc_free(&tmp);
  }

#ifdef DEBUG_OUTPUT
  {
    strlist varnames;
    strlist_init(&varnames, '\0');
    get_keys(&vars, &varnames);
    buffer_puts(buffer_2, "varnames: ");
    strlist_dump(buffer_2, &varnames);
    output_all_vars(out, &vars, &varnames);
  }
#endif

  if(str_equal(tools.make, "gmake")) {
    strlist_nul(&vpath);
    // buffer_putm_internal(out,   "\nvpath ", vpath.sa.s,   "\n", NULL);
    stralloc_replacec(&vpath.sa, ' ', ':');
    buffer_putm_internal(out, "VPATH = ", vpath.sa.s, "\n\n", NULL);
    buffer_flush(out);
  }

  if(ninja) {
    output_build_rules(out, "cc", &commands.compile);
    output_build_rules(out, "link", &commands.link);
    output_build_rules(out, "lib", &commands.lib);
    put_newline(out, 0);
  }

  {
    MAP_PAIR_T t;
    MAP_FOREACH(rules, t) {
      const char* name = MAP_ITER_KEY(t);
      target* rule = MAP_ITER_VALUE(t);
      if(rule->recipe.len)
        continue;
      if(str_end(name, exts.obj)) {

        if(!str_end(tools.make, "make"))
          stralloc_weak(&rule->recipe, &commands.compile);
      }

#if DEBUG_OUTPUT_
      buffer_puts(buffer_2, "Empty RULE '");
      buffer_puts(buffer_2, name);
      buffer_putc(buffer_2, '\'');
      buffer_putnlflush(buffer_2);
#endif
    }
  }

  if(batch || shell) {
    if(batch) {
      buffer_putm_internal(out, "CD %~dp0", newline, NULL);
    } else {
      buffer_putm_internal(out, "cd \"$(dirname \"$0\")\"\n\n", NULL);
    }
    output_script(out, NULL);
  } else {
    output_all_rules(out);
  }

quit : {
  strlist deps;
  strlist_init(&deps, '\0');
  MAP_PAIR_T t;
  MAP_FOREACH(sourcedirs, t) {
    sourcedir* sdir = *(sourcedir**)MAP_ITER_VALUE(t);
    if(1 /* && set_size(&sdir->deps)*/) {
      strlist_zero(&deps);
      sourcedir_deps(sdir, &deps);

#ifdef DEBUG_OUTPUT_
      buffer_putm_internal(buffer_2, "source directory '", MAP_ITER_KEY(t), "' deps =\n", NULL);
      strlist_dump(buffer_2, &deps);
      buffer_putnlflush(buffer_2);
#endif
    }
  }
}

  {
    struct dnode* link;
    strlist deps;
    strlist_init(&deps, '\0');
    dlist_foreach_down(&sourcelist, link) {
      sourcefile* source = dlist_data(link, sourcefile*);
      if(0 && 1) {
        buffer_putm_internal(buffer_2, "source: ", source->name, " deps: ", NULL);
        strlist_zero(&deps);
        sources_deps(source, &deps);
        buffer_puts(buffer_2, " includes: ");
        strlist_dump(buffer_2, &source->includes);
        buffer_putnlflush(buffer_2);
      }
    }
  }

  // MAP_DESTROY(sourcedirs);
  return ret;
}
