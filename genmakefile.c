#include <assert.h>
#include <stdlib.h>
#include <ctype.h>
#include "lib/stralloc.h"
#include "lib/buffer.h"
#include "genmakefile.h"
#include "map.h"
#include "debug.h"
#include "mplab.h"
#include "lib/unix.h"
#include "lib/sig.h"

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

static int cmd_objs = 0, cmd_libs = 0, cmd_bins = 0;
static stralloc preprocess_command, compile_command, lib_command, link_command, mkdir_command, delete_command;
static const char* libpfx = DEFAULT_LIBPFX;
static const char *make_begin_inline, *make_sep_inline, *make_end_inline;
static const char* comment = "#";
static const char* cross_compile = "";
static stralloc output_name;
static strlist vpath;
static stralloc srcdir;
static char pathsep_make = DEFAULT_PATHSEP, pathsep_args = DEFAULT_PATHSEP;
static const char* quote_args = "";
static strlist build_as_lib;
static strlist include_dirs, link_libraries;
static strlist pptoks;
static MAP_T sourcedirs, targetdirs, rules, vars;
static slink* sourcelist;
static const char* newline = "\n";
static int batch, shell, ninja;
static int batchmode;
static linklib_fmt* format_linklib_fn;
static int inst_bins, inst_libs;
static int cygming;

set_t srcs;
exts_t exts = {DEFAULT_OBJEXT, DEFAULT_LIBEXT, DEFAULT_EXEEXT, DEFAULT_PPSEXT};
dirs_t dirs;
tools_t tools;
config_t cfg = {.mach = {0, 0}, .sys = {0, 0}, .chip = {0, 0, 0}, .build_type = 1, .lang = LANG_CXX};

#ifdef _DEBUG
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
#endif
#ifndef _WIN32
#define _mkdir mkdir
#endif

void
concat_quoted(stralloc* sa, const char* x, size_t len) {
  size_t j, k;
  if(quote_args[0] == '\0') {
    stralloc_catb(sa, x, len);
    return;
  }

  for(j = 0; j < len;) {
    k = byte_chr(&x[j], len - j, ' ');
    if(k) {
      if(j)
        stralloc_catc(sa, ' ');
      stralloc_cats(sa, quote_args);
      stralloc_catb(sa, &x[j], k);
      stralloc_cats(sa, quote_args);
    }
    j += k + 1;
  }
}
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
    if(!stralloc_endb(prefix, &pathsep_make, 1) && !stralloc_endb(prefix, ")", 1))
      stralloc_catc(out, pathsep_make);
  }
  stralloc_catb(out, x, n);
  stralloc_nul(out);
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
  stralloc_nul(out);
}

void
path_prefix_sa(const stralloc* prefix, stralloc* sa) {
  if(prefix->len && !stralloc_equals(prefix, ".")) {
    stralloc_insertb(sa, prefix->s, 0, prefix->len);
    if(!stralloc_endb(prefix, &pathsep_make, 1))
      stralloc_insertb(sa, &pathsep_make, prefix->len, 1);
  }
  stralloc_nul(sa);
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
 * @brief path_output  Convert source file name to object file name
 * @param in
 * @param out
 * @return
 */
char*
path_output(const char* in, stralloc* out, const char* ext) {
  stralloc* prefix;
  prefix = path_is_absolute_sa(&dirs.build.sa) ? &dirs.work.sa : &dirs.build.sa;
  path_prefix_b(prefix, "", 0, out);
  return path_extension(str_basename(in), out, ext);
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
      x++;
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

char*
path_clean_s(const char* path) {
  if(str_startb(path, dirs.this.sa.s, dirs.this.sa.len)) {
    path += dirs.this.sa.len;
    if(path[0] == PATHSEP_C)
      path++;
  }
  while(str_start(path, "./")) path += 2;
  return (char*)path;
}

char*
path_clean_b(const char* path, size_t* len) {
  if(byte_startb(path, *len, dirs.this.sa.s, dirs.this.sa.len)) {
    path += dirs.this.sa.len;
    *len -= dirs.this.sa.len;
    if(*len > 0 && path[0] == PATHSEP_C) {
      path++;
      (*len)--;
    }
  }
  while(byte_starts(path, *len, "./")) {
    path += 2;
    (*len) -= 2;
  }
  return (char*)path;
}

const char*
path_mmap_read(const char* path, size_t* n) {
  const char* x;
  stralloc sa;
  stralloc_init(&sa);
  if(dirs.this.sa.s) {
    path_prefix_s(&dirs.this.sa, path, &sa);
    stralloc_replacec(&sa, pathsep_make, PATHSEP_C);
  }
  if((x = mmap_read(sa.s, n)) == NULL) {
    errmsg_warnsys("error opening '", path, "'", 0);
    buffer_putnlflush(buffer_2);
  }
  stralloc_free(&sa);
  return x;
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
 * @brief extract_pptok  Extract preprocessor tokens directives
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
            /*          buffer_puts(buffer_2, "pptoks: ");
                      buffer_put(buffer_2, x, linelen);
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
 * @brief format_linklib_lib  Output library name (+".lib")
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
size_t
skip_comment(const char* p, size_t len) {
  return byte_finds(p, len, "*/");
}

/**
 * @brief main_scan  Checks if the given source file contains a main() function
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
        /*        if(i > 4 && !isspace(*(x - 5)))
                  continue;
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
 * @brief main_present  Checks for main() routine in source file
 * @param filename  Path to source file
 * @return          1 when yes, 0 when no, -1 on error
 */
int
main_present(const char* filename) {
  char* x;
  size_t n;
  if((x = (char*)path_mmap_read(filename, &n))) {
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
#ifdef DEBUG_OUTPUT_
    debug_sa("dirs.work: ", &dirs.work.sa);
    debug_sa("dirs.build: ", &dirs.build.sa);
    debug_sa("dirs.out: ", &dirs.out.sa);
    debug_sa("dirs.this: ", &dirs.this.sa);
    debug_str("dir: ", dir);
    buffer_putnlflush(buffer_2);
#endif
    stralloc_zero(&arg);
    stralloc_cats(&arg, dir);
    //    path_relative(dir, dirs.this.sa.s, &arg);
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
 * @brief includes_extract  Extract #include directives
 * @param x
 * @param n
 * @param includes
 * @param sys
 */
void
includes_extract(const char* x, size_t n, strlist* includes, int sys) {
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
  if((x = path_mmap_read(srcfile, &n))) {
    includes_extract(x, n, includes, sys);
    mmap_unmap(x, n);
    return 1;
  }
  return 0;
}

void
includes_add(const char* dir) {
  static stralloc abs;
  stralloc_zero(&abs);
  path_canonical(dir, &abs);
  if(strlist_push_unique_sa(&include_dirs, &abs)) {
#ifdef DEBUG_OUTPUT
    buffer_puts(buffer_2, "Added to include_dirs: ");
    buffer_putsa(buffer_2, &abs);
    buffer_putnlflush(buffer_2);
#endif
  }
}

/**
 * @brief includes_to_libs  Include list to library list
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
      path_concat(libpfx, str_len(libpfx), sa.s, sa.len, &sa);
    }
    path_concat(dirs.this.sa.s, dirs.this.sa.len, sa.s, sa.len, &sa);
    //  debug_sa("include", &sa);
    stralloc_zero(&lib);
    stralloc_copys(&lib, path_basename(sa.s));
    if(stralloc_endb(&lib, ".h", 2))
      lib.len -= 2;

    stralloc_cats(&lib, exts.lib);
    // debug_sa("includes_to_libs", &lib);
    if((rule = rule_find_sa(&lib))) {
#if DEBUG_OUTPUT_
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
 * Find or create rule
 */
target*
rule_get(const char* name) {
  target* ret = NULL;
  MAP_PAIR_T t = NULL;
  size_t len = str_len(name);
  if(!MAP_SEARCH(rules, name, len + 1, &t)) {
    target tgt;
    byte_zero(&tgt, sizeof(struct target_s));
    tgt.name = str_ndup(name, len); //*/ MAP_KEY(t);
    set_init(&tgt.output, 0);
    set_adds(&tgt.output, name);
    set_init(&tgt.prereq, 0);
    MAP_INSERT(rules, name, len + 1, &tgt, ((sizeof(struct target_s) + 3) / 4) * 4);
    (MAP_SEARCH(rules, name, len + 1, &t));
    // ret = MAP_VALUE(t);
#ifdef DEBUG_OUTPUT_I
    if(t) {
      buffer_putm_internal(buffer_2, "Created rule '", ((target*)MAP_VALUE(t))->name, "'\n", NULL);
      buffer_flush(buffer_2);
    }
#endif
  }
  if(t)
    ret = MAP_VALUE(t);
  return ret;
}

/**
 * @brief rule_get_sa
 * @param name
 * @return
 */
target*
rule_get_sa(stralloc* name) {
  stralloc_nul(name);
  return rule_get(name->s);
}

/**
 * @brief rule_find
 * @param needle
 * @return
 */
target*
rule_find(const char* needle) {
  MAP_PAIR_T t;
  MAP_FOREACH(rules, t) {
    const char* name = MAP_KEY(t);
    if(str_equal(name, needle))
      return MAP_VALUE(t);
    if(str_equal(path_basename((char*)name), path_basename((char*)needle)))
      return MAP_VALUE(t);
    /*
        if(t->next == rules->list_tuple)
          break; */
  }
  return 0;
}

void
rule_rename(target* rule, const char* name) {
  size_t len;
  stralloc out;
  stralloc_init(&out);
  stralloc_catset(&out, &rule->output, " ");

  len = byte_rchr(out.s, out.len, pathsep_make);
  if(out.len > len)
    out.len = len + 1;
  else
    out.len = 0;
  set_adds(&rule->output, name);
  free((char*)rule->name);
  rule->name = str_ndup(out.s, out.len);
}

/**
 * @brief rule_find_sa
 * @param name
 * @return
 */
target*
rule_find_sa(stralloc* name) {
  stralloc_nul(name);
  return rule_find(name->s);
}

/**
 * @brief rule_find_b
 * @param x
 * @param n
 * @return
 */
target*
rule_find_b(const char* x, size_t n) {
  target* r;
  stralloc sa;
  stralloc_init(&sa);
  sa.s = (char*)x;
  sa.len = n;
  r = rule_find_sa(&sa);
  return r;
}

/**
 * @brief rule_match
 * @param rule           Target rule
 * @param pattern        Preqreq wildcard pattern
 * @return               1 if match
 */
int
rule_match(target* rule, const char* pattern) {
  int ret = 0;
  const char* s;
  size_t n;
  set_iterator_t it;
  stralloc sa;
  stralloc_init(&sa);
  set_foreach(&rule->prereq, it, s, n) {
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

          concat_quoted(out, rule->name, str_len(rule->name));
          byte_replace(&out->s[p], out->len - p, pathsep_args == '/' ? '\\' : '/', pathsep_args);
          break;
        }
        case '^': {
          concat_quoted(out, prereq, plen);
          break;
        }
        case '|': {
          stralloc_subst(out, prereq, plen, " ", make_sep_inline ? make_sep_inline : "\n ");
          break;
        }
        case '<': {
          size_t n = byte_chr(prereq, plen, ' ');
          concat_quoted(out, prereq, n);
          break;
        }
      }
      i++;
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
  set_iterator_t it;
  strlist prereq;
  strlist_init(&prereq, ' ');
  if(stralloc_contains(&rule->recipe, "-+$^")) {
    //    pfx = "-+";
  }
  set_foreach(&rule->prereq, it, s, len) {
    if(pfx) {
      strlist_push(&prereq, pfx);
      stralloc_catb(&prereq.sa, s, len);
    } else {
      strlist_pushb_unique(&prereq, s, len);
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
        n--;
      rule_command_subst(rule, out, r.start, n);
      if(r.start + n < r.end && r.start[n] == ' ')
        n++;
      r.start += n;
    }
  } else if(str_start(tools.make, "g")) {
    stralloc_copy(out, &rule->recipe);
  } else {
    rule_command_subst(rule, out, prereq.sa.s, prereq.sa.len);
  }
  stralloc_free(&prereq.sa);
}

/**
 * @brief rule_add_dep
 * @param t
 * @param other
 * @return
 */
int
rule_add_dep(target* t, target* other) {
  target** ptr;

  if((ptr = array_find(&t->deps, sizeof(target*), &other)) == NULL) {
    array_catb(&t->deps, &other, sizeof(other));
    array_foreach_t(&other->deps, ptr) { rule_add_dep(t, *ptr); }
    return 1;
  }
  return 0;
}

/**
 * @brief rule_add_deps
 * @param t
 * @param deps
 */
void
rule_add_deps(target* t, const strlist* deps) {
  const char* x;
  size_t n;
  strlist_foreach(deps, x, n) {
    target* other;
    if(str_len(t->name) == n && !str_diffn(t->name, x, n))
      continue;
    if((other = rule_find_b(x, n))) {
      rule_add_dep(t, other);
    }
  }
}

/**
 * @}
 */
/**
 * @brief rule_dep_list_recursive   Lists all dependencies of a target
 * @param l                           Output target names
 * @param t                           Target
 */
static uint32 rule_dep_serial;

void
rule_dep_list_recursive(target* t, set_t* s, int depth, strlist* hier) {
  target** ptr;
  t->serial = rule_dep_serial;
  array_foreach_t(&t->deps, ptr) {
    const char* name = (*ptr)->name;
    if(t->serial == (*ptr)->serial)
      continue;
    if(!strlist_contains(hier, name)) {
      strlist_push(hier, name);
      rule_dep_list_recursive(*ptr, s, depth + 1, hier);
      strlist_pop(hier);
      if(depth >= 0) {
        if(set_adds(s, name)) {
        }
      }
    }
  }
}

/**
 * @brief rule_dep_list
 * @param l
 * @param t
 */
void
rule_dep_list(target* t, set_t* s) {

  strlist hier;
  strlist_init(&hier, '\0');
  strlist_push(&hier, t->name);

  set_clear(s);

  --rule_dep_serial;

  rule_dep_list_recursive(t, s, 0, &hier);
  strlist_free(&hier);
}

/**
 * @brief indirect_dep_list  List all indirect deps of a target
 * @param l                  Output target names
 * @param t                  Target
 */
void
rule_deps_indirect(target* t, set_t* s) {
  target** ptr;
  strlist hier;
  strlist_init(&hier, '\0');
  strlist_push(&hier, t->name);

  set_adds(s, t->name);

  array_foreach_t(&t->deps, ptr) {
    if(*ptr)
      rule_dep_list_recursive(*ptr, s, 0, &hier);
  }

  //  set_deletes(s, t->name);
  strlist_free(&hier);
}

/**
 * @brief rule_dump
 * @param rule
 */
void
rule_dump(target* rule) {
#ifdef DEBUG_OUTPUT
  buffer_putm_internal(buffer_2, "Rule '", rule->name, "'\n", NULL);
  if(set_size(&rule->prereq)) {
    buffer_puts(buffer_2, "  prereq: ");
    buffer_putset(buffer_2, &rule->prereq, " ", 1);
  }
  if(set_size(&rule->output)) {
    buffer_puts(buffer_2, "  output: ");
    buffer_putset(buffer_2, &rule->output, " ", 1);
  }
  if(rule->recipe.len) {
    stralloc_nul(&rule->recipe);
    buffer_putm_internal(buffer_2, "  recipe: ", rule->recipe.s, "\n", NULL);
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
 * Add a path to a strlist
 */
void
add_path_b(set_t* s, const char* path, size_t len) {
  if(set_has(s, path, len))
    return;
  set_add(s, path, len);
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
    path_prefix_s(&srcdir, path, &sa);

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
    path_prefix_s(&srcdir, path, &sa);

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
  path_concat(dir->s, dir->len, path, pathlen, &sa);
  sa.len = path_collapse(sa.s, sa.len);
  add_path_sa(s, &sa);
  stralloc_free(&sa);
}

/**
 * @defgroup source functions
 * @{
 */
static int
is_b(const char* x, size_t n, int (*fn_b)(const char*, size_t)) {
  return fn_b(x, n);
}

static int
is_s(const char* s, int (*fn_b)(const char*, size_t)) {
  return is_b(s, str_len(s), fn_b);
}

static int
is_sa(const stralloc* sa, int (*fn_b)(const char*, size_t)) {
  return is_b(sa->s, sa->len, fn_b);
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

int
is_source_b(const char* filename, size_t len) {
  if(byte_ends(filename, len, ".c"))
    return 1;
  if(byte_ends(filename, len, ".S"))
    return 1;
  if(byte_ends(filename, len, ".s"))
    return 1;
  if(byte_ends(filename, len, ".asm"))
    return 1;
  if(cfg.lang != LANG_CXX)
    return 0;
  if(byte_ends(filename, len, ".C"))
    return 1;
  if(byte_ends(filename, len, ".cc"))
    return 1;
  if(byte_ends(filename, len, ".cpp"))
    return 1;
  if(byte_ends(filename, len, ".cxx"))
    return 1;
  if(byte_ends(filename, len, ".c++"))
    return 1;
  return 0;
}

int
is_source_sa(stralloc* sa) {
  return is_sa(sa, &is_source_b);
}

int
is_source(const char* s) {
  return is_s(s, &is_source_b);
}

int
is_filename_b(const char* filename, size_t len) {
  size_t dirpos = byte_rchrs(filename, len, "/\\", 2);
  size_t extpos = byte_rchr(filename, len, '.');
  if(dirpos < len)
    return 1;
  if(extpos + 1 < len) {
    size_t i;
    for(i = extpos + 1; i < len; i++)
      if(!isalnum(filename[i]))
        return 0;
    return 1;
  }
  return 0;
}

int
is_filename_sa(stralloc* sa) {
  return is_sa(sa, &is_filename_b);
}

int
is_filename(const char* s) {
  return is_s(s, &is_filename_b);
}

int
is_include_b(const char* filename, size_t len) {
  if(byte_ends(filename, len, ".h"))
    return 1;
  if(cfg.lang != LANG_CXX)
    return 0;
  if(byte_ends(filename, len, ".H"))
    return 1;
  if(byte_ends(filename, len, ".hh"))
    return 1;
  if(byte_ends(filename, len, ".hpp"))
    return 1;
  if(byte_ends(filename, len, ".hxx"))
    return 1;
  if(byte_ends(filename, len, ".h++"))
    return 1;
  return 0;
}

int
is_include_sa(stralloc* sa) {
  return is_sa(sa, &is_include_b);
}

int
is_include(const char* s) {
  return is_s(s, &is_include_b);
}

int
is_object_b(const char* filename, size_t len) {
  return byte_ends(filename, len, exts.obj);
}

int
is_object_sa(stralloc* sa) {
  return is_sa(sa, &is_object_b);
}

int
is_object(const char* s) {
  return is_s(s, &is_object_b);
}

/**
 * @brief sources_new  Create new source file entry.
 * @param name
 * @return
 */
sourcefile*
sources_new(const char* name) {
  sourcefile* ret;
  if((ret = (sourcefile*)malloc(sizeof(sourcefile)))) {
    byte_zero(ret, sizeof(sourcefile));
    ret->name = str_dup(name);
    ret->has_main = is_source(ret->name) && main_present(ret->name) == 1;
#if DEBUG_OUTPUT_
    if(ret->main_present)
      debug_str("Source has main()", ret->name);
#endif
    set_init(&ret->pptoks, 0);
    // slist_pushb(&sources, &ret, sizeof(ret));
    return ret;
  }
  return 0;
}

/**
 * @brief sources_add  Adds a source file to the given list.
 * @param filename
 * @param sources
 */
int
sources_add(const char* source) {
  return sources_add_b(source, str_len(source));
}

int
sources_add_b(const char* x, size_t len) {
  int ret = 0;

  if(byte_chr(x, len, '/') == len && byte_ends(x, len, "strlist_shift.c"))
    asm("int3");

  if(is_source_b(x, len) || is_include_b(x, len)) {
    if(len > dirs.this.sa.len && byte_startb(x, len, dirs.this.sa.s, dirs.this.sa.len)) {
      size_t dirlen = dirs.this.sa.len + 1;
      x += dirlen;
      len -= dirlen;
    }
    ret = set_add(&srcs, x, len);
#ifdef DEBUG_OUTPUT_
    if(ret)
      debug_byte("sources_add", x, len);
#endif
  }
  return 0;
}

int
sources_sort(const char** a, const char** b) {
  size_t alen = str_rchrs(*a, "/\\", 2);
  size_t blen = str_rchrs(*b, "/\\", 2);
  int er, rdir, rfile;
  const char *ext_a, *ext_b;
  ext_a = *a + str_rchr(*a, '.');
  ext_b = *b + str_rchr(*b, '.');
  er = str_diff(ext_a, ext_b);
  if(er)
    return er;
  rdir = str_diffn(*a, *b, alen < blen ? blen : alen);
  rfile = str_diff(path_basename(*a), path_basename(*b));
  if(rdir == 0)
    return rfile;
  return rdir;
}

/**
 * @brief sources_get Searches all source files in the given directory and creates a string-array.
 * @param basedir
 * @param sources
 */
void
sources_get(const char* basedir) {
  rdir_t rdir;
#ifdef DEBUG_OUTPUT_
  buffer_puts(buffer_2, "sources_get: ");
  buffer_puts(buffer_2, basedir);
  buffer_putnlflush(buffer_2);
#endif
  if(!rdir_open(&rdir, basedir)) {
    const char* s;
    while((s = rdir_read(&rdir))) {
      size_t len = str_len(s);
      if(len + 1 > dirs.this.sa.len && byte_equal(s, dirs.this.sa.len, dirs.this.sa.s) &&
         path_is_separator(s[dirs.this.sa.len])) {
        s += dirs.this.sa.len + 1;
      }
      if(sources_add(s)) {
      }
      {
#ifdef DEBUG_OUTPUT_
        buffer_puts(buffer_2, "Added source: ");
        buffer_puts(buffer_2, s);
        buffer_putnlflush(buffer_2);
#endif
      }
    }
  }
}

const char*
sources_find(const char* name, size_t len, size_t* cptr) {
  char* x;
  const char* ret = 0;
  set_iterator_t it;
  size_t i, n, count = 0;
#ifdef DEBUG_OUTPUT_
  buffer_puts(buffer_2, "Find source: ");
  buffer_put(buffer_2, name, len);
  buffer_putnlflush(buffer_2);
#endif
  set_foreach(&srcs, it, x, n) {
    i = byte_findb(x, n, name, len);
    if(i == n)
      i = byte_findb(x, n, path_basename(name), len);
    if(i < n) {
      if(!ret && is_source(x))
        ret = x;
      count++;
    }
  }
  if(cptr)
    *cptr = count;
  if(ret) {
#ifdef DEBUG_OUTPUT_
    buffer_puts(buffer_2, "Found source: ");
    buffer_puts(buffer_2, ret);
    buffer_puts(buffer_2, "(query = '");
    buffer_put(buffer_2, name, len);
    buffer_puts(buffer_2, " ', ");
    buffer_putulong(buffer_2, count);
    buffer_puts(buffer_2, " results)");
    bufferfer_putnlflush(buffer_2);
#endif
  }
  return ret;
}

void
sources_deps(sourcefile* file, strlist* out) {
  const char* x;
  size_t len;
  set_iterator_t it, it2;
  sourcefile* src;
  sourcedir* dir;
  strlist_foreach(&file->includes, x, len) { strlist_pushb_unique(out, x, len); }

  set_foreach(&file->deps, it, x, len) {
    stralloc_nul(&srcdir);
#if DEBUG_OUTPUT_I_q
    buffer_puts(buffer_2, "sources_deps '");
    buffer_puts(buffer_2, file->name);
    buffer_puts(buffer_2, "' ");
    buffer_put(buffer_2, x, len);
    buffer_putnlflush(buffer_2);
#endif
    if((dir = sourcedir_findb(x, len))) {
      const char* s;
      size_t n;

      set_foreach(&dir->deps, it2, s, n) { strlist_pushb_unique(out, s, n); }
      slist_foreach(dir->sources, src) {
#if DEBUG_OUTPUT_
        buffer_puts(buffer_2, "sources_deps");
        buffer_puts(buffer_2, " s: ");
        buffer_puts(buffer_2, src->name);
        buffer_putnlflush(buffer_2);
#endif
        strlist_push_unique(out, src->name);
      }
    }
  }
}

void
sources_readdir(stralloc* dir, strarray* out) {
  rdir_t d;
  stralloc srcdir;
  stralloc_init(&srcdir);
  stralloc_copy(&srcdir, dir);
  stralloc_nul(&srcdir);
#if DEBUG_OUTPUT_
  buffer_puts(buffer_2, "sources_readdir: ");
  buffer_putsa(buffer_2, dir);
  buffer_putnlflush(buffer_2);
#endif
  // path_concat(dirs.this.sa.s, dirs.this.sa.len, dir->s, dir->len, &srcdir);
  if(!rdir_open(&d, srcdir.s)) {
    const char* s;
    while((s = rdir_read(&d))) {
      if(!is_source(s) && !is_include(s))
        continue;
      if(str_start(s, dirs.out.sa.s))
        s += dirs.out.sa.len;
      else
        s += dir->len;
      if(*s == PATHSEP_C)
        s++;
      /*  buffer_puts(buffer_2, "rdir_read: ");
        buffer_puts(buffer_2, s);
        buffer_putnlflush(buffer_2);*/
      strarray_push_unique(out, s);
    }
  }
  stralloc_free(&srcdir);
}

void
sources_addincludes(sourcefile* file, sourcedir* sdir, const strlist* includes, strarray* sources) {
  const char* x;
  size_t n;
  stralloc basedir, dir, path, real, relative;
  strlist directories;
  stralloc_init(&real);
  stralloc_init(&dir);
  stralloc_init(&basedir);
  stralloc_init(&path);
  stralloc_init(&relative);
  path_dirname(file->name, &basedir);
  path_absolute_sa(&basedir);
  strlist_init(&directories, '\0');
  strlist_init(&file->includes, '\0');
  stralloc_nul(&basedir);
  if(stralloc_starts(&basedir, dirs.this.sa.s))
    stralloc_remove(&basedir, 0, dirs.this.sa.len + 1);
  stralloc_copy(&relative, &basedir);
  stralloc_nul(&relative);
#if DEBUG_OUTPUT_
  {
    buffer_puts(buffer_2, "relative: ");
    buffer_putsa(buffer_2, &relative);
    buffer_putnlflush(buffer_2);
    buffer_puts(buffer_2, "dirs.out.sa: ");
    buffer_putsa(buffer_2, &dirs.out.sa);
    buffer_putnlflush(buffer_2);
    buffer_puts(buffer_2, "basedir: ");
    buffer_putsa(buffer_2, &basedir);
    buffer_putnlflush(buffer_2);
  }
#endif

  relative.s = path_clean_b(relative.s, &relative.len);

  strlist_foreach(includes, x, n) {
    size_t len = n;
    //  stralloc_copyb(&path, x,len);
    //  stralloc_zero(&path);
    path_concat(relative.s, relative.len, x, len, &path);
    path_collapse_sa(&path);
    path_concat(dirs.this.sa.s, dirs.this.sa.len, path.s, path.len, &real);
    path_canonical_sa(&real);
    path_collapse_sa(&real);

    {
      const char* s = "strlist_shift.c";
      if(stralloc_endsb(&real, s, str_len(s)))
        asm("int3");
    }

#if DEBUG_OUTPUT_
    {
      buffer_puts(buffer_2, "source_addincludes: file=");
      buffer_puts(buffer_2, file->name);
      buffer_puts(buffer_2, " path=");
      buffer_putsa(buffer_2, &path);
      buffer_putnlflush(buffer_2);
    }
#endif

    strlist_pushb_unique(&file->includes, path.s, path.len);
    strarray_pushb_unique(sources, path.s, path.len);

    if(len >= 2 && real.s[real.len - 2] == '.') {
      stralloc_copyb(&dir, path.s, path.len - 2);
      stralloc_nul(&dir);
      real.len -= 2;
      path.len -= 2;
      stralloc_nul(&path);
      stralloc_nul(&real);
      if(stralloc_diff(&basedir, &dir)) {
#if DEBUG_OUTPUT_
        buffer_puts(buffer_2, "real = ");
        buffer_putsa(buffer_2, &real);
        buffer_puts(buffer_2, " path = ");
        buffer_putsa(buffer_2, &path);
        buffer_puts(buffer_2, " dir = ");
        buffer_putsa(buffer_2, &dir);
        buffer_puts(buffer_2, " basedir = ");
        buffer_putsa(buffer_2, &basedir);
        buffer_putnlflush(buffer_2);
#endif
        if(path_exists(real.s) && path_is_directory(real.s) /* && !stralloc_equal(&real, &dirs.this.sa)*/) {
          if(1) /*strlist_push_unique_sa(&directories, &path))*/ {
            strarray a;
            strarray_init(&a);
            sources_readdir(&real, &a);
            sources_get(real.s);
            set_addsa(&file->deps, &path);
            set_addsa(&sdir->deps, &path);
#if DEBUG_OUTPUT_
            buffer_puts(buffer_2, "path = ");
            buffer_putsa(buffer_2, &path);
            buffer_putnlflush(buffer_2);
#endif
          }
        }
      }
      stralloc_cats(&real, ".c");
      stralloc_nul(&real);

      if(path_exists(real.s) && !path_is_directory(real.s)) {
        stralloc_cats(&path, ".c");
        stralloc_nul(&path);
#if DEBUG_OUTPUT_
        buffer_puts(buffer_2, "exists: ");
        buffer_putsa(buffer_2, &path);
        buffer_putnlflush(buffer_2);
#endif
        strarray_pushb_unique(sources, path.s, path.len);
      }
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
 * @brief var_isset
 * @param name
 * @return
 */
int
var_isset(const char* name) {
  return !!MAP_GET(vars, name, str_len(name) + 1);
}

/**
 * @brief var_list  Find or create variable
 * @param name
 * @return
 */
strlist*
var_list(const char* name) {
  MAP_PAIR_T t;
  if(!MAP_SEARCH(vars, name, str_len(name) + 1, &t)) {
    strlist var;
    strlist_init(&var, (name[0] >= 'A' && name[0] <= 'Z') ? ' ' : pathsep_args);
    MAP_INSERT(vars, name, str_len(name) + 1, &var, sizeof(strlist));
    MAP_SEARCH(vars, name, str_len(name) + 1, &t);
  }
  return (strlist*)MAP_VALUE(t);
}

/**
 * @brief var
 * @param name
 * @return
 */
const char*
var_get(const char* name) {
  strlist* v = var_list(name);
  stralloc_nul(&v->sa);
  return v->sa.s;
}

/**
 * @brief var_set  Set variable
 * @param name
 * @param value
 * @return
 */
strlist*
var_set(const char* name, const char* value) {
  strlist* var;
  var = var_list(name);
  stralloc_zero(&var->sa);
  stralloc_copys(&var->sa, value);
  return var;
}

void
var_unset(const char* name) {
  if(var_isset(name))
    MAP_DELETE(vars, (void*)name, str_len(name) + 1);
}

/**
 * @brief var_push  Add value to variable
 * @param name
 * @param value
 */
void
var_push(const char* name, const char* value) {
  strlist* var = var_list(name);
  strlist_push_unique(var, value);
}

/**
 * @brief var_push_sa
 * @param name
 * @param value
 */
void
var_push_sa(const char* name, stralloc* value) {
  strlist_push_unique_sa(var_list(name), value);
}

/**
 * @brief push_lib  Add library spec to variable
 * @param name
 * @param lib
 */
void
push_lib(const char* name, const char* lib) {
  stralloc sa;
  strlist* var = var_list(name);
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
 * @brief get_rules_by_cmd  Search rules by command
 * @param cmd
 * @param deps
 */
void
get_rules_by_cmd(stralloc* cmd, strlist* deps) {
  MAP_PAIR_T t;
  MAP_FOREACH(rules, t) {
    target* rule = MAP_VALUE(t);
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

void
sourcedir_addsource(const char* source, strarray* srcs) {
  char** p;
  stralloc r, dir, tmp;
  strlist l;
  size_t i, n, dlen;
  const char *x, *s;
  set_iterator_t it;
  set_t* set;
  sourcedir* srcdir;
  sourcefile* file = sources_new(source);
  stralloc_init(&dir);
  stralloc_init(&tmp);
  strlist_init(&l, '\0');
  stralloc_init(&r);
  strlist_zero(&l);
  path_dirname(source, &dir);
  stralloc_nul(&dir);
  dlen = dir.len;

  source = path_clean_s(source);

  srcdir = sourcedir_getsa(&dir);
  slist_add(&srcdir->sources, &file->link);
  slist_pushb(&sourcelist, &file, sizeof(sourcefile*));
  ++srcdir->n_sources;

  if(!path_exists(source)) {
#ifdef DEBUG_OUTPUT
    buffer_puts(buffer_2, "Path doesn't exist: ");
    buffer_puts(buffer_2, source);
    buffer_putnlflush(buffer_2);
#endif
    return;

    asm("int3");
  }

  if((x = path_mmap_read(source, &n)) != 0) {
    includes_extract(x, n, &l, 0);
    extract_pptok(x, n, &file->pptoks);
    mmap_unmap(x, n);
  }

  sources_addincludes(file, srcdir, &l, srcs);

  //  sourcedir_deps(srcdir, &file->deps);
  /*
    set_foreach(&file->deps, s, i) {
      set_add(&srcdir->deps, s, i);
      sourcedir_deps_b(s, i, &srcdir->deps);
    }*/
  set_foreach_it(&file->pptoks, it) {
    x = set_iterator_value(&it, &n);
    if(n >= 5 && (byte_equal(x, 4, "USE_") || byte_equal(x, 5, "HAVE_")))

      set_add(&srcdir->pptoks, x, n);
  }
  /*
    strlist_dump(buffer_2, &srcdir->pptoks);
    strlist_dump(buffer_2, &file->pptoks);*/
  /*    strlist_foreach(&l, x, n) {
        stralloc_zero(&tmp);
        path_concat()
        strlist_pushb(&file->includes, x, n);
       path_concat(thisdir.sa.s, thisdir.sa.len, )
      }*/
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
  // debug_sa("srcdir", &dir);
  // debug_sa("includes", &srcdir->includes);
  stralloc_free(&r);
  strlist_free(&l);
  stralloc_free(&dir);
}

/**
 * @defgroup source dir functions
 * @{
 */
/**
 * @brief sourcedir_find  Searches for a source directory
 * @param path           Path string
 * @return               Pointer to sourcedir structure or NULL
 */
sourcedir*
sourcedir_find(const char* path) {
  sourcedir** ptr;
  if((ptr = MAP_GET(sourcedirs, path, str_len(path) + 1)))
    return *ptr;
  return 0;
}

/**
 * @brief sourcedir_findsa Searches for a source directory
 * @param path             Path stralloc
 * @return                 Pointer to sourcedir structure or NULL
 */
sourcedir*
sourcedir_findsa(stralloc* path) {
  sourcedir** ptr;
  stralloc_nul(path);
  if((ptr = MAP_GET(sourcedirs, path->s, path->len + 1)))
    return *ptr;
  return 0;
}

/**
 * @brief sourcedir_findb  Searches for a source directory
 * @param x                Path buffer
 * @param n                Length of path
 * @return               Pointer to sourcedir structure or NULL
 */
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
    MAP_NODE_T found;

    newdir = alloc_zero(sizeof(sourcedir));
    set_init(&newdir->pptoks, 0);
    // set_init(&newdir.includes, 0);

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

/**
 * @brief sourcedir_populate  Creates a hash-map of all source directories
 * @param srcs
 * @param sourcedirs
 */
void
sourcedir_populate(strarray* srcs) {
  char** p;
  MAP_PAIR_T t;
  size_t i;
  strlist d;
  const char* x;
  size_t n;
  strlist_init(&d, '\0');

  /*strarray_foreach(srcs, p) {
    sourcedir_addsource(*p, srcs);
  }*/
  /*
    for(i = 0; i < strarray_size(srcs); i++) {
      const char* s = strarray_at(srcs, i);

      sourcedir_addsource(s, srcs);
    }*/
  MAP_FOREACH(sourcedirs, t) {
    const char* name = MAP_KEY(t);
    sourcedir* dir = *(sourcedir**)MAP_VALUE(t);
    sourcefile* file;
#ifdef DEBUG_OUTPUT_
    buffer_puts(buffer_2, "sourcedir: ");
    buffer_puts(buffer_2, name);
    buffer_putnlflush(buffer_2);
#endif
    strlist_zero(&d);
    slist_foreach(dir->sources, file) {
      sources_deps(file, &d);
      strlist_foreach(&d, x, n) { set_add(&file->deps, x, n); }
    } /*
     sourcedir_deps(dir, &d);
     set_foreach(&d, x, n) { set_add(&dir->deps, x, n); }*/
  }
  MAP_FOREACH(sourcedirs, t) {
    sourcedir* dir = *(sourcedir**)MAP_VALUE(t);
    strlist_zero(&d);
    sourcedir_deps(dir, &d);

    strlist_foreach(&d, x, n) { set_add(&dir->deps, x, n); }
  }
}

/**
 * @brief sourcedir_dump_all
 * @param b
 * @param sourcedirs
 */
void
sourcedir_dump_all(buffer* b) {
  MAP_PAIR_T t;

  MAP_FOREACH(sourcedirs, t) {
    sourcedir* srcdir = *(sourcedir**)MAP_VALUE(t);
    slink* link;
    sourcefile* pfile;

    buffer_puts(b, " '");
    buffer_put(b, MAP_KEY(t), str_len(MAP_KEY(t)));
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

/**
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

      if(set_adds(deplist, name))
        print_rule_deps_r(b, (*ptr), deplist, hierlist, depth + 1);
    }
  }

  hierlist->sa.len = l;
  //   strlist_trunc(hierlist, depth);
}

/**
 * @brief print_rule_deps  Prints dependency tree for a target
 * @param b                  Output buffer
 * @param t                  Target
 */
void
print_rule_deps(buffer* b, target* t) {
  const char* s;
  size_t n, nb;
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
    sourcedir* srcdir = *(sourcedir**)MAP_VALUE(t);
    target* lib;
    size_t n;
    const char* s;

    stralloc_zero(&sa);
    path_prefix_s(&dirs.work.sa, str_basename(MAP_KEY(t)), &sa);
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

      //  debug_set("indir", &indir, " ");
      /*  strlist_sub(&indir, &libs);

        strlist_sub(&libs, &indir);*/
#if DEBUG_OUTPUT_
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
gen_single_rule(stralloc* output, strlist* prereq, stralloc* cmd) {
  target* rule;
  char* x;
  size_t n;

  if((rule = rule_get_sa(output))) {
    strlist_foreach(prereq, x, n) { set_add(&rule->prereq, x, n); }
    /*rule->prereq.sep = prereq->sep;

    set_adds(&rule->prereq, prereq);*/

    if(cmd && !(rule->recipe.len && stralloc_equal(&rule->recipe, cmd))) {
      if(rule->recipe.len)
        stralloc_cats(&rule->recipe, "\n");

      stralloc_cat(&rule->recipe, cmd);
    }
  }
  return rule;
}

/**
 * @brief gen_clean_rule  Generate clean rule which removes all target outputs
 * @param rules
 */
void
gen_clean_rule() {
  target* rule;
  static stralloc output;

  /* Generate "clean" rule */
  if((rule = rule_get("clean"))) {
    MAP_PAIR_T t;
    char* arg;
    size_t cmdoffs, lineoffs = 0;
    stralloc fn;
    strlist delete_args;
    stralloc_init(&fn);
    strlist_init(&delete_args, '\0');

    if(delete_command.len == 0)
      stralloc_copys(&delete_command, /* cfg.sys.type == OS_WIN ? "DEL /F /Q" : */ "$(RM)");

    cmdoffs = delete_command.len;

    MAP_FOREACH(rules, t) {

      const char* target = MAP_KEY(t);
      stralloc_zero(&output);

      /* Ignore the dirs.build rule */
      if(stralloc_equals(&dirs.work.sa, MAP_KEY(t)))
        continue;

      rule = MAP_VALUE(t);

      if(target[str_chr(target, '%')]) {
        //   stralloc_nul(&rule->output.sa);
        stralloc_catset(&output, &rule->output, " ");
      }

      /* If the rule has prerequisites and a recipe, it must be a producing rule */
      if(set_size(&rule->prereq) && rule->recipe.s) {
        size_t bpos;

        if((MAP_KEY(t))[(bpos = str_rchr(MAP_KEY(t), '{'))]) {
          size_t epos = str_rchr(&(MAP_KEY(t))[bpos + 1], '}');

          stralloc_zero(&fn);
          stralloc_catb(&fn, &(MAP_KEY(t)[bpos + 1]), epos);
          stralloc_catc(&fn, pathsep_make);
          stralloc_cats(&fn, "*");
          stralloc_catb(&fn, &(MAP_KEY(t)[bpos + 1 + epos + 1]), str_chr(&(MAP_KEY(t)[bpos + 1 + epos + 1]), ':'));
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

int
filter_pptoks(const void* x, size_t n) {
  return byte_equal(x, n > 4 ? 4 : n, "USE_");
}

/**
 * @brief gen_srcdir_compile_rules  Generate compile rules for every source file given
 * @param rules                     All rules
 * @param srcdir                    source dir structure
 * @param dir                       source dir path
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
  path_output("%", &target, exts.obj);
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
    path_prefix_s(&srcdir, src->name, &srcs);
    path_wildcard(&srcs, "%");
    stralloc_replacec(&srcs, pathsep_make == '/' ? '\\' : '/', pathsep_make);

    stralloc_zero(&obj);
    path_output(src->name, &obj, exts.obj);

    if(str_start(tools.make, "g") || ((shell | batch) && batchmode)) {
      stralloc_cat(&target, &srcs);
      // path_wildcard(&target, "%");
    } else if(batchmode) {
      stralloc_zero(&target);
      stralloc_catm_internal(&target, "{", dir, "}", ext, "{", dirs.work.sa.s, "}", exts.obj, ":", 0);
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
        // rule->recipe = malloc(sizeof(stralloc));
        stralloc_init(&rule->recipe);
        stralloc_copy(&rule->recipe, &compile_command);
        stralloc_replaces(&rule->recipe, "-Fo", "-Fd");
        stralloc_replaces(&rule->recipe, "$@", dirs.work.sa.s);

      } else {
        size_t p, e;
        char* x;

        if(dirs.work.sa.len == 0 || stralloc_equals(&dirs.work.sa, ".")) {
          stralloc_copy(&rule->recipe, &compile_command);
          x = stralloc_begin(&rule->recipe);
          // e = stralloc_end(&rule->recipe);

          p = e = stralloc_finds(&rule->recipe, "$@");
          while(p > 0 && !((x[p + 1] == '/' || x[p + 1] == '-') && x[p] == ' ')) p--;

          e += 2;
          if(x[e] == '"')
            e++;

          stralloc_remove(&rule->recipe, p, e - p);
        } else {
          stralloc_weak(&rule->recipe, &compile_command);
        }

        //        rule->recipe = /* str_start(tools.make, "g") ? NULL :*/ &compile_command;
      }
    }
  }

  stralloc_free(&obj);
  stralloc_free(&target);
  stralloc_free(&defines);

#ifdef DEBUG_OUTPUT
  MAP_FOREACH(rules, t) {
    rule = MAP_DATA(t);
    rule_dump(rule);
  }
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
gen_simple_compile_rules(sourcedir* srcdir, const char* dir, const char* fromext, const char* toext, stralloc* cmd) {
  sourcefile* src;
  stralloc ppsrc, obj;
  stralloc_init(&ppsrc);
  stralloc_init(&obj);

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
      path_output(base, &obj, ".pp.c");

      //      stralloc_inserts(&ppsrc, ".pp", byte_rchr(ppsrc.s, ppsrc.len, '.'));

      if((rule = rule_get_sa(&obj))) {
        add_source(&rule->prereq, src->name);
        stralloc_copy(&rule->recipe, &preprocess_command);

        ppsrc = obj;
        srcname = ppsrc.s;
      }
    }

    stralloc_init(&obj);
    path_output(base, &obj, toext);

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
 * @brief gen_srcdir_lib_rule  Generate lib rule for source dir
 * @param rules                     All rules
 * @param srcdir                    source dir structure
 * @param dir                       source dir path
 * @return
 */
target*
gen_srcdir_lib_rule(sourcedir* srcdir, const char* name) {
  target *dep = 0, *rule;
  stralloc sa;
  stralloc_init(&sa);

  path_prefix_s(&dirs.work.sa, name, &sa);
  // stralloc_copys(&sa, name);

  stralloc_cats(&sa, exts.lib);

#if DEBUG_OUTPUT_
  debug_str("gen_srcdir_lib_rule", name);
  debug_sa("gen_srcdir_lib_rule", &sa);
#endif

  if((str_start(tools.make, "g") || batchmode) && cfg.mach.arch != PIC) {
    debug_str("gen_srcdir_compile_rules", name);
    dep = gen_srcdir_compile_rules(srcdir, name);
  } else {
    /* if(0 && tools.preproc) {
       gen_simple_compile_rules(rules, srcdir, name, ".c", exts.pps, &preprocess_command);
       dep = gen_simple_compile_rules(rules, srcdir, name, exts.pps, exts.obj, &compile_command);
     } else {*/
    dep = gen_simple_compile_rules(srcdir, name, ".c", exts.obj, &compile_command);
    //}
  }

  if((rule = rule_get_sa(&sa))) {
    sourcefile* pfile;
    set_init(&rule->prereq, 0);

    if(dep) {
      size_t n;
      char* s;
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

          path_output(pfile->name, &sa, exts.obj);

        set_addsa(&rule->prereq, &sa);
      }
    }

    stralloc_weak(&rule->recipe, &lib_command);

#ifdef DEBUG_OUTPUT_
    rule_dump(rule);
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
gen_srcdir_rule(sourcedir* sdir, const char* name) {
  sourcefile* src;
  target* rule;
  stralloc mask;
  stralloc_init(&mask);

  slist_foreach(sdir->sources, src) {

    const char* s;
#if DEBUG_OUTPUT_
    debug_str("sourcefile", src->name);
#endif
    if(!is_source(src->name))
      continue;

    s = str_ndup(src->name, str_rchr(src->name, '.'));

    stralloc_zero(&mask);
    path_prefix_s(&dirs.work.sa, str_basename(s), &mask);
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
gen_lib_rules() {
  MAP_PAIR_T t;
  target* all;
  stralloc inc, abspath;
  stralloc_init(&inc);
  stralloc_init(&abspath);

  all = rule_get("all");

  MAP_FOREACH(sourcedirs, t) {
    target* rule;
    sourcedir* srcdir = *(sourcedir**)MAP_VALUE(t);
    const char *s, *base = path_basename(MAP_KEY(t));
    size_t n;

    if(str_equal(base, ".")) {
      stralloc_zero(&abspath);
      path_absolute(MAP_KEY(t), &abspath);
      stralloc_nul(&abspath);
      base = path_basename(MAP_KEY(t));
    }
#if DEBUG_OUTPUT_
    debug_str("srcdir", MAP_KEY(t));
    debug_str("base", base);
#endif
    if(strlist_contains(&build_as_lib, base) /* || (str_equal(base, "lib") && cfg.mach.arch != PIC)*/ ||
       base[0] == '.' || base[0] == '\0')
      continue;

    // gen_srcdir_rule(rules, srcdir, base);

    rule = gen_srcdir_lib_rule(srcdir, base);

    strlist_push_unique(&link_libraries, rule->name);

    set_adds(&all->prereq, rule->name);

    array_catb(&srcdir->rules, &rule, sizeof(target*));
  }
  stralloc_free(&inc);
  stralloc_free(&abspath);
}

/**
 * @brief gen_link_rules  Generate compile rules for every source file with a main()
 * @param rules
 * @param sources
 * @return
 */
int
gen_link_rules(/*strarray* sources*/) {
  int num_main = 0, count = 0;
  target *all, *preprocess, *compile, *link;
  const char *x, *link_lib;
  char **p, *filename;
  size_t n;
  set_iterator_t it;
  set_t incs, deps;
  strlist libs, indir;
  stralloc dir, ppsrc, obj, bin;
  sourcefile* file;
  sourcedir* srcdir;

  set_init(&incs, 0);
  set_init(&deps, 0);
  strlist_init(&libs, ' ');
  strlist_init(&indir, ' ');
  stralloc_init(&dir);
  stralloc_init(&ppsrc);
  stralloc_init(&obj);
  stralloc_init(&bin);
  all = rule_get("all");

  slist_foreach(sourcelist, file) {
    filename = (char*)file->name;

    set_clear(&incs);
    strlist_zero(&libs);
    set_clear(&deps);
    strlist_zero(&indir);

    stralloc_zero(&ppsrc);
    stralloc_zero(&obj);

    if(is_source(filename) && main_present(filename)) {
      path_dirname(filename, &dir);
      srcdir = sourcedir_getsa(&dir);
      // gen_compile_rules(srcdir, dir.s);
      if(tools.preproc) {
        path_output(filename, &ppsrc, exts.pps);
      }
      path_output(filename, &obj, exts.obj);
      if(tools.preproc && (preprocess = rule_get_sa(&ppsrc))) {
        add_source(&preprocess->prereq, filename);
        stralloc_weak(&preprocess->recipe, &preprocess_command);
      }
      if((compile = rule_get_sa(&obj))) {
        size_t len;
        const char* x;
        add_source(&compile->prereq, filename);
        stralloc_weak(&compile->recipe, &compile_command);
        /*        stralloc_nul(&incs);
                buffer_putm_internal(buffer_2, "rule '", compile->name, "' includes: ", incs.sa.s, NULL);
                buffer_putnlflush(buffer_2);
        */
      }
      stralloc_zero(&bin);

      if(!cmd_libs) {
        stralloc outname;
        stralloc_init(&outname);
        stralloc_cats(&outname, path_basename(filename));
        if(stralloc_endb(&outname, ".c", 2))
          outname.len -= 2;
        stralloc_nul(&outname);
        path_output(outname.s, &bin, exts.bin);
      } else {
        path_extension(obj.s, &bin, exts.bin);
      }

      add_path_b(&all->prereq, bin.s, bin.len);

      if((link = rule_get_sa(&bin))) {
        int nremoved;
        sourcefile* pfile;
        num_main++;
        add_path_sa(&link->prereq, &obj);

        if(cmd_libs) {
          slist_foreach(srcdir->sources, pfile) {
            if(!pfile->has_main) {
              size_t n;
              const char* i;
              stralloc_zero(&obj);
              path_output(pfile->name, &obj, exts.obj);

              add_path_sa(&link->prereq, &obj);
            }
          }
        } else {
          slink* source2;
          const char* i;
          size_t n;
          slist_foreach(sourcelist, source2) {
            sourcefile* sfile = slist_data(source2);
            char* filename = (char*)sfile->name;

            stralloc_zero(&dir);
            path_dirname(filename, &dir);

            if(str_end(filename, ".h"))
              continue;

            strlist_push_unique_sa(&vpath, &dir);

            stralloc_zero(&obj);
            path_output(filename, &obj, exts.obj);

            add_path_sa(&link->prereq, &obj);
          }
        }

        //  get_rules_by_cmd(&lib_command, &link->prereq);

        stralloc_weak(&link->recipe, &link_command);

        // debug_str("program", link->name);
        // debug_sa("program libs", &libs.sa);

        /*        deps_indirect(&indir, &libs);

                strlist_sub(&libs, &indir);

                rule_add_deps(link, &libs);

                strlist_zero(&deps);
                rule_dep_list(&deps, link);

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
        set_clear(&deps);
        rule_dep_list(link, &deps);

        strlist_foreach(&link_libraries, link_lib, n) {
          const char* s;
          size_t len;
          target* lib = rule_find_b(link_lib, n);

          set_foreach(&lib->prereq, it, s, len) { set_add(&deps, s, len); }
          //  set_cat(&deps, &lib->prereq);
          add_path(&all->prereq, lib->name);
          //          strlist_push(&deps, link_lib);
        }
        if(set_size(&deps)) {
          set_foreach(&deps, it, x, n) { set_add(&link->prereq, x, n); }
        }

#if 0 // def DEBUG_OUTPUT_
                /*print_rule_deps(buffer_2, link);
                buffer_putm_internal(buffer_2, "Deps for executable '", link->name, "': ", NULL);
                buffer_putsa(buffer_2, &deps.sa);
                buffer_putnlflush(buffer_2);*/
#endif

        ++count;
      }
    }
  }

  if(num_main == 1 && link && output_name.len) {
    stralloc oldname;
    size_t pos;
    stralloc_init(&oldname);
    stralloc_catset(&oldname, &link->output, " ");

    if(!stralloc_endsb(&output_name, exts.bin, str_len(exts.bin)))
      stralloc_cats(&output_name, exts.bin);

    stralloc_nul(&output_name);
    rule_rename(link, output_name.s);

    if(set_has_sa(&all->prereq, &oldname)) {
    }

    /*  if((pos = stralloc_find(&all->prereq.sa, &oldname)) < all->prereq.sa.len) {
        stralloc_replace(&all->prereq.sa, pos, oldname.len, link->output.sa.s, link->output.sa.len);
      }*/
  }

  set_free(&incs);
  strlist_free(&libs);
  set_free(&deps);
  strlist_free(&indir);
  stralloc_free(&bin);
  stralloc_free(&obj);
  stralloc_free(&dir);
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
    target* rule = MAP_VALUE(t);
    int do_lib, do_bin;

    do_lib = (inst_libs && (str_end(MAP_KEY(t), ".lib") || str_end(MAP_KEY(t), ".a") ||
                            MAP_KEY(t)[str_find(MAP_KEY(t), ".so")] || rule->recipe.s == lib_command.s));

    do_bin =
        (inst_bins && (str_end(MAP_KEY(t), ".dll") || str_end(MAP_KEY(t), ".exe") || rule->recipe.s == link_command.s));

    if(!(do_lib || do_bin))
      continue;

    if(!inst) {
      inst = rule_get("install");

      // inst->recipe = malloc(sizeof(stralloc));
      stralloc_init(&inst->recipe);

      set_adds(&inst->prereq, "all");
    }

    if(!var_isset("prefix")) {

      var_set("prefix", "/usr");
      stralloc_catm_internal(&inst->recipe, newline, "\t$(INSTALL_DIR) $(DESTDIR)$(prefix)", 0);

      if(!v) {
        v = var_set("INSTALL", "install")->sa.s;

        var_set("INSTALL_DIR", str_start(v, "install") ? "$(INSTALL) -d" : "mkdir");

        if(do_lib)
          var_set("INSTALL_DATA", str_start(v, "install") ? "$(INSTALL) -m 644" : "$(INSTALL)");
      }
    }

    if(do_bin) {
      if(!var_isset("bindir")) {
        var_set("bindir", "$(prefix)/bin");
        stralloc_catm_internal(&inst->recipe, newline, "\t$(INSTALL_DIR) $(DESTDIR)$(bindir)", 0);
      }

      var_set("INSTALL_EXEC", str_start(v, "install") ? "$(INSTALL) -m 755" : "$(INSTALL)");

      stralloc_catm_internal(&inst->recipe, newline, "\t$(INSTALL_EXEC) ", MAP_KEY(t), " $(DESTDIR)$(bindir)", 0);
    }

    if(do_lib) {
      if(!var_isset("libdir")) {
        var_set("libdir", "$(prefix)/lib");
        if(str_end(tools.compiler, "64")) {
          var_push("libdir", "$(X64)");
        }

        stralloc_catm_internal(&inst->recipe, newline, "\t$(INSTALL_DIR) $(DESTDIR)$(libdir)", 0);
      }

      stralloc_catm_internal(&inst->recipe, newline, "\t$(INSTALL_DATA) ", MAP_KEY(t), " $(DESTDIR)$(libdir)", 0);
    }
  }
  return inst;
}

void
get_keys(MAP_T* map, strlist* list) {
  MAP_PAIR_T t;
  MAP_FOREACH(*map, t) { strlist_push(list, MAP_KEY(t)); }
}

int
input_command(stralloc* cmd, char* argv[]) {
  size_t n, len;
  int compile = 0, link = 0, objects = 0;
  const char *x, *file;
  char** p;
  sourcedir* srcdir;
  stralloc output, dir;
  strlist files, flags;
  stralloc_init(&output);
  stralloc_init(&dir);
  strlist_init(&files, ' ');
  strlist_init(&flags, ' ');

  for(p = argv; (len = *p ? str_len(*p) : 0, x = *p); p++) {
    int src = is_source_b(x, len);
    if(byte_equal(x, 2, "-c") || src) {
      compile = 1;
      link = 0;
    }
    if(src)
      sources_add_b(x, byte_chrs(x, len, " \t\n\v\r", 5));
  }

  for(p = argv; (len = *p ? str_len(*p) : 0, x = *p); p++) {
    len = str_len(x);
    if(len >= 2 && !byte_diff(x, 2, "-o")) {
      size_t i;
      if(len > 2) {
        x += 2;
        len -= 2;
      } else {
        x = *++p;
        len = str_len(x);
      }
      if(len)
        stralloc_copyb(&output, x, len);
    }
  }

  for(p = argv; (len = *p ? str_len(*p) : 0, x = *p); p++) {
    len = str_len(x);
    strlist_pushb(&flags, x, len);

    if(len >= 2 && byte_equal(x, 2, "-o")) {
      x += (x[2] == '\0') ? 3 : 2;
    } else if(len >= 3 && byte_equal(x, 3, "-MF")) {
      size_t i = len > 3 ? 3 : 4;
      x += i;
      len -= i;
    } else if(len >= 1 && x[0] != '-') {
      if(is_filename_b(x, len))
        strlist_pushb(&files, x, len);
    }
  }

  for(p = argv; (len = *p ? str_len(*p) : 0, x = *p); p++) {
    len = str_len(x);
    if(byte_finds(x, len, exts.obj) < len || byte_finds(x, len, exts.lib) < len) {
      objects++;

      if(objects > 1) {
        compile = 0;
        link = 1;
      }
    }
  }

  if(output.len == 0) {
    stralloc sa;
    stralloc_init(&sa);
    stralloc_copyb(&sa, files.sa.s, byte_chr(files.sa.s, files.sa.len, files.sep));
    stralloc_nul(&sa);
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

  if(output.len) {
    char* d;
    uint32* count_ptr;
    stralloc_nul(&output);
    stralloc_zero(&dir);
    d = path_dirname(output.s, &dir);
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
    /// path_prefix_s(&dirs.this.sa, path_basename(output.s), &source);
    stralloc_copy(&source, &output);
    if(is_object_sa(&source)) {
      size_t extpos;
      for(;;) {
        extpos = byte_rchr(source.s, source.len - 1, '.');
        if(extpos == source.len)
          break;
        if(source.s[extpos + byte_chrs(&source.s[extpos], source.len - extpos, "/\\", 2)])
          break;
        source.len = extpos + 1;
        if(is_source_sa(&source))
          break;
      }
      if(extpos < source.len) {
        extpos++;
        source.len = extpos;
      }
    }

    stralloc_nul(&source);
    x = source.s;
    n = source.len;
    file = NULL;

    while(n > 0) {
      size_t i;
      file = sources_find(x, n, &len);

      if(file && len == 1)
        break;
      if(file && len >= 1)
        break;

      i = byte_chrs(x, n, "\\/", 2);
      if(i == n)
        break;
      if(i + 1 < n)
        i++;
      x += i;
      n -= i;
    }

    if(file) {
      // if(n == 1)
      stralloc_zero(&files.sa);
      strlist_push_unique(&files, file);
    }
    stralloc_free(&source);
  }
  stralloc_nul(&output);

  path_prefix_sa(&dirs.work.sa, &output);

  n = count_b(&files, &is_source_b);
  if(n > 0)
    compile = 1;

  if((n || count_b(&files, &is_object_b)) || output.len) {
#if DEBUG_OUTPUT_
    buffer_puts(buffer_2, link ? "link" : compile ? "compile" : "other");
    buffer_puts(buffer_2, " rule\n  output = '");
    buffer_putsa(buffer_2, &output);
    buffer_puts(buffer_2, "'\n  command = '");
    buffer_putsa(buffer_2, cmd);
    buffer_puts(buffer_2, "'\n  files = ");
    strlist_dump(buffer_2, &files);
    buffer_putnlflush(buffer_2);
#endif

    stralloc_catc(cmd, ' ');
    stralloc_cat(cmd, &flags.sa);
    stralloc_nul(cmd);
    {
      stralloc* sacmd = alloc_zero(sizeof(stralloc));
      stralloc_move(sacmd, cmd);

      gen_single_rule(&output, &files,
                      sacmd); // link ? &link_command : compile ? &compile_command : &lib_command);
    }
  }

  stralloc_free(&output);
  strlist_free(&files);

  return link ? 2 : compile ? 1 : 0;
}

int
input_command_line(const char* x, size_t n) {
  size_t idx = 0;
  int ret;
  char** av;
  stralloc command;
  strlist args;
  stralloc_init(&command);
  strlist_init(&args, '\0');
  while(n > 0) {
    size_t i;
    if((i = scan_whitenskip(x, n)) == n)
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
    i = scan_nonwhitenskip(x, n);
    if(idx == 0)
      stralloc_copyb(&command, x, i);
    else
      strlist_pushb(&args, x, i);
    x += i;
    n -= i;
    idx++;
  }
  if(*(av = strlist_to_argv(&args)))
    ret = input_command(&command, av);
  if(av)
    alloc_free(av);
  stralloc_free(&command);
  strlist_free(&args);
  return ret;
}

/**
 * @brief output_all_vars  Output all variables
 * @param b
 * @param vars
 */
void
output_var(buffer* b, MAP_T* vars, const char* name) {
  stralloc v;
  strlist* var;
  set_t refvars;
  const char* ref;
  size_t len;
  set_iterator_t it;
  MAP_PAIR_T t = 0;
  set_init(&refvars, 0);
  if(MAP_SEARCH(*vars, name, str_len(name) + 1, &t)) {
    stralloc_init(&v);
    var = MAP_VALUE(t);
    if(var->sa.len) {
      stralloc_copys(&v, MAP_KEY(t));
      if(ninja)
        stralloc_lower(&v);
      stralloc_nul(&v);

      set_clear(&refvars);
      extract_vars(var->sa.s, var->sa.len, &refvars);

      set_foreach(&refvars, it, ref, len) { output_var(b, vars, ref); }

      if(batch)
        buffer_putm_internal(b, "@SET ", v.s, "=", NULL);
      else if(shell)
        buffer_putm_internal(b, v.s, "=\"", NULL);
      else
        buffer_putm_internal(b, v.s, " = ", NULL);

      if(ninja || shell) {
        stralloc_zero(&v);
        var_subst(&var->sa, &v, "$", "", 1);
        buffer_putsa(b, &v);
      } else if(batch) {
        stralloc_zero(&v);
        var_subst(&var->sa, &v, "%", "%", 1);
        buffer_putsa(b, &v);
      } else {
        buffer_putsa(b, &var->sa);
      }
      if(shell)
        buffer_putc(b, '"');
      put_newline(b, 0);
      buffer_flush(b);
    }

    stralloc_free(&v);

    MAP_DELETE(*vars, MAP_KEY(t), str_len(MAP_KEY(t)));
  }
  set_free(&refvars);
}

/**
 * @brief output_all_vars  Output all variables
 * @param b
 * @param vars
 */
void
output_all_vars(buffer* b, MAP_T* vars, strlist* varnames) {
  MAP_PAIR_T t;
  const char* name;
  stralloc_nul(&varnames->sa);
  strlist_foreach_s(varnames, name) { output_var(b, vars, name); }
  put_newline(b, 1);
}

/**
 * @brief output_make_rule  Output rule to buffer
 * @param b
 * @param rule
 */
#define YELLOW "\x1b[1;33m"
#define RED "\x1b[1;31m"
#define NC "\x1b[0m"
void
output_make_rule(buffer* b, target* rule) {
  const char* x;
  static stralloc output, sa, name;
  size_t n, num_prereqs, num_outputs;

  num_prereqs = set_size(&rule->prereq);
  num_outputs = set_size(&rule->output);
  debug_nl = "\n";
  stralloc_zero(&name);
  stralloc_copys(&name, rule->name);

#ifdef DEBUG_OUTPUT_
  if(str_end(rule->name, ".a") || rule->name[str_chr(rule->name, '%')]) {
    const char* color = str_end(rule->name, ".a") ? YELLOW : RED;

    buffer_puts(buffer_2, color);
    debug_str("rule->name" NC, rule->name);
    if(num_outputs <= 1) {
      buffer_puts(buffer_2, color);
      debug_set("rule->output" NC, &rule->output, " ");
    }
    buffer_puts(buffer_2, color);
    debug_int("num_outputs" NC, num_outputs);
    if(num_prereqs <= 1) {
      buffer_puts(buffer_2, color);
      debug_set("rule->prereq" NC, &rule->prereq, " ");
    }
    buffer_puts(buffer_2, color);
    debug_int("num_prereqs" NC, num_prereqs);
  }
#endif
  /* if(array_length(&rule->deps, sizeof(target*)))
     print_rule_deps(b, rule);
  */

  if(num_prereqs == 0 && str_diffn(rule->name, dirs.work.sa.s, dirs.work.sa.len) &&
     !rule->name[str_chr(rule->name, pathsep_make)] && str_end(rule->name, ":")) {
    buffer_putm_internal(b, ".PHONY: ", rule->name, newline, NULL);
  }

  stralloc_zero(&sa);
  stralloc_catset(&sa, &rule->output, " \\\n");

  strlist_foreach(&rule->vars, x, n) {
    // buffer_puts(b, "output: ");
    buffer_putsa(b, &sa);
    buffer_puts(b, ": ");
    buffer_put(b, x, n);
    buffer_puts(b, "\n\n");
  }
  stralloc_zero(&output);

  if(sa.len > 0)
    stralloc_cat(&output, &sa);

  if(stralloc_diffs(&sa, rule->name)) {
    size_t outlen;

    if((outlen = output.len))
      stralloc_cats(&output, ": \\\n");
    stralloc_cat(&output, &name);
    if(!outlen)
      stralloc_cats(&output, ": ");
  } else {
    stralloc_cats(&output, ": ");
  }

  if(num_prereqs) {
    const char* str;
    size_t len;
    set_iterator_t it;

    set_foreach(&rule->prereq, it, str, len) {
      if(stralloc_endsb(&output, str, len))
        continue;

      stralloc_catc(&output, ' ');
      stralloc_catb(&output, str, len);
    }

    // stralloc_replacec(&output, pathsep_make == '/' ? '\\' : '/', pathsep_make);
  }

  buffer_putsa(b, &output);
  buffer_flush(b);
  // stralloc_zero(&output);

  if(rule->recipe.s) {
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
      buffer_putset(b, &rule->prereq, " ", 1);
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
  else if(stralloc_equal(&rule->recipe, &lib_command))
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
#warning rule->prereq.sa

    stralloc_catset(&path, &rule->prereq, " ");

    stralloc_replacec(&path, pathsep_args == '/' ? '\\' : '/', pathsep_args == '/' ? '/' : '\\');

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
output_all_rules(buffer* b) {
  MAP_PAIR_T t;

  MAP_FOREACH(rules, t) {
    target* rule = MAP_VALUE(t);
    const char* name = MAP_KEY(t);

    if(!cmd_libs && str_end(name, ".a"))
      continue;

#ifdef DEBUG_OUTPUT_
    buffer_puts(buffer_2, "Outputting rule '");
    buffer_putsx(buffer_2, MAP_KEY(t), MAP_KEY(t) _len);
    buffer_putc(buffer_2, '\'');
    buffer_putnlflush(buffer_2);
#endif

    if(ninja)
      output_ninja_rule(b, MAP_VALUE(t));
    else
      output_make_rule(b, MAP_VALUE(t));
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
    MAP_PAIR_T t;
    ++serial;

    /*    MAP_FOREACH(rules, t) {
          rule = MAP_VALUE(t);

          output_script(b, rule);
        }
    */
    flush = 1;
    rule = rule_get("all");
  }

  if(rule->serial == serial)
    return;

  if(!rule->name[str_chr(rule->name, '%')]) {
    if(rule->recipe.s != compile_command.s)
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
 * @brief set_system  Set the system type
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

  // pathsep_args = cfg.sys.type == NTOS ? '\\' : '/';
  // pathsep_make = cfg.sys.type == NTOS ? '\\' : '/';

  return ret;
}

/**
 * @brief set_make_type  Set tools.make program type
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

  stralloc_copys(&mkdir_command, cfg.sys.os == OS_WIN ? "IF NOT EXIST $@ MKDIR $@" : "mkdir -p $@");

  if(str_start(tools.make, "batch") || str_start(tools.make, "cmd")) {
    pathsep_args = '\\';
    pathsep_make = '\\';

  } else if(str_start(tools.make, "bmake") || str_start(tools.make, "borland")) {

    /* Borland LANG_C++ Builder Make */
    pathsep_make = '\\';
    make_begin_inline = "@&&|\r\n ";
    make_sep_inline = " ";
    make_end_inline = "\r\n|";
    inst = "copy /y";

    newline = "\r\n";

  } else if(str_start(tools.make, "nmake")) {

    /* Microsoft NMake */
    pathsep_make = '\\';
    /*    make_begin_inline = "@<<$*.rsp\r\n";
        make_end_inline = "\r\n<<keep";
    */
    newline = "\r\n";

    inst = "copy /y";

  } else if(str_start(tools.make, "gmake") || str_start(tools.make, "gnu")) {

    newline = "\n";
    pathsep_make = '/';
    stralloc_copys(&mkdir_command, "test -d $@ || mkdir -p $@");
    stralloc_copys(&delete_command, "rm -f");

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
  }

  if(inst_bins || inst_libs)
    var_set("INSTALL", inst);

  pathsep_args = pathsep_make;

  return 1;
}

/**
 * @brief set_tools.compiler_type Set the tools.compiler type
 * @param tools.compiler
 * @return
 */
int
set_compiler_type(const char* compiler) {

  var_set("CC", "cc");
  var_set("CXX", "c++");

  stralloc_copys(&compile_command, "$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(DEFS) -c -o $@ $<");
  set_command(&lib_command, "$(LIB) /out:$@", "$^");
  set_command(&link_command,
              "$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(LDFLAGS) $(EXTRA_LDFLAGS) -o $@",
              "$^ $(LIBS) $(EXTRA_LIBS) $(STDC_LIBS)");

  set_command(&preprocess_command, "$(CPP) $(CPPFLAGS) $(DEFS) -o$@", "$<");

  //  var_push("DEFS", "-DHAVE_ERRNO_H=1");

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
    /*    var_push("LDFLAGS",
                 "/DEBUG /DYNAMICBASE /INCREMENTAL /NXCOMPAT /TLBID:1");
    */
    // var_push("LDFLAGS", "/SUBSYSTEM:CONSOLE /TLBID:1 /DYNAMICBASE /NXCOMPAT");

    //  var_push("LDFLAGS", "/MANIFEST /manifest:embed2 /MANIFESTUAC:\"level=asInvoker uiAccess=false\"");

    stralloc_copys(&compile_command, "$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(DEFS) -c -Fo\"$@\" $<");
    set_command(&lib_command, "$(LIB) -out:$@", "$^");
    //    stralloc_copys(&lib_command, "$(LIB) /OUT:$@ @<<\n\t\t$^\n<<");

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

      //      stralloc_copys(&compile_command, "$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(DEFS) -c -Fo $@ $<");
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

    set_command(&link_command,
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

    set_command(&lib_command, "$(AR) rcs $@", "$^");
    // set_command(&link_command, "$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(LDFLAGS) $(EXTRA_LDFLAGS) -o $@", "$^ $(LIBS)
    // $(EXTRA_LIBS)");
    set_command(&link_command, "$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(LDFLAGS) $(EXTRA_LDFLAGS) -o $@", "$^ $(LIBS)");

    exts.bin = "";

    format_linklib_fn = &format_linklib_switch;

    /*
     * Borland C++ Builder
     */
  } else if(str_start(compiler, "bcc")) {

    // pathsep_args = '\\';

    //    var_push("DEFS", "-DWIN32_LEAN_AND_MEAN");
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

      /*  if(cfg.build_type != BUILD_TYPE_DEBUG)
          var_push("CFLAGS", "-Or");
      */
      set_command(&link_command, "$(CC) $(LDFLAGS) $(EXTRA_LDFLAGS) -o $@ ", "$^ $(LIBS) $(EXTRA_LIBS) $(STDC_LIBS)");

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

      stralloc_copys(&compile_command, "$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(DEFS) -c -o$@ $<");
      set_command(&link_command, "$(CC) $(LDFLAGS) $(EXTRA_LDFLAGS) -e$@", "$^ $(LIBS) $(EXTRA_LIBS) $(STDC_LIBS)");
    }

    var_set("LINK", "ilink32");
    var_set("LIB", "tlib");

    push_lib("STDC_LIBS", "cw32");
    push_lib("STDC_LIBS", "import32");

    set_command(&lib_command, "$(LIB) /p256 $@ /u", "$^");

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
        &link_command,
        "$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(LDFLAGS) $(EXTRA_LDFLAGS) -o $@ $^ $(LIBS) $(EXTRA_LIBS) $(STDC_LIBS)");
    stralloc_copys(&link_command, "$(LINK) $(LDFLAGS) $(EXTRA_LDFLAGS) -o $@ $^ $(LIBS) $(EXTRA_LIBS) $(STDC_LIBS)");

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

    //  var_push("LDFLAGS", "-Wl,-subsystem=console");

    if(cfg.build_type == BUILD_TYPE_MINSIZEREL)
      //      var_push("LDFLAGS", "-Wl,-file-alignment=16");

      var_push("CFLAGS", "-Wall");
    var_push("CPPFLAGS", "-D__TCC__=1");

    set_command(&lib_command, "$(AR) r $@", "$^");
    set_command(&link_command, "$(CC) $(LDFLAGS) $(EXTRA_LDFLAGS) -o $@", "$^ $(LIBS) $(EXTRA_LIBS)");

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

    //    stralloc_copys(&compile_command, "$(CC) /! /c $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(DEFS) -o$@ \"/I;\"
    //    $<");
    stralloc_copys(&compile_command, "$(CC) /! /c $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(DEFS) -o$@ $<");
    set_command(&lib_command, "$(LIB) /! $@", "$^");
    set_command(&link_command, "$(LINK) -c /! $(LDFLAGS) $(EXTRA_LDFLAGS) -o$@", "$^ c0xpe.o $(LIBS) $(DEFAULT_LIBS)");

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
    //    set_command(&lib_command, "$(LIB) -c $@", "$^");
    set_command(&lib_command, "$(LIB) -c $@", "$^");
    stralloc_copys(&compile_command, "$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(DEFS) -c -o$@ $<");
    set_command(&link_command,
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

    var_push("CFLAGS", "-Ze"); /* Activates Microsoft's extensions to C */
    // var_push("CFLAGS", "-Zx"); /* Activates Pelle's extensions to C */
    var_push("CFLAGS", "-Go"); /* Accepts 'old' names for C runtime functions */
    // var_push("CFLAGS", "-Gz"); /* default to __stdcall */

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

    /*    if(cfg.build_type == BUILD_TYPE_MINSIZEREL)
          var_push("CFLAGS", "-Os");

        else*/
    if(cfg.build_type == BUILD_TYPE_DEBUG || cfg.build_type == BUILD_TYPE_RELWITHDEBINFO) {
      var_push("CFLAGS", "-Zi");
      var_push("LDFLAGS", "-DEBUG");
    }

    stralloc_copys(&compile_command, "$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(DEFS) -c $< -Fo$@");
    stralloc_copys(&link_command, "$(CC) $^ -Fe $@ $(LDFLAGS) $(EXTRA_LDFLAGS) $(LIBS) $(EXTRA_LIBS) $(STDC_LIBS)");

    pathsep_args = '\\';
    quote_args = "\"";

  } else if(str_start(compiler, "sdcc")) {
    var_set("CC", "sdcc");
    var_set("LINK", "sdcc");
    var_set("LIB", "sdar");
    var_unset("CXX");

    cfg.mach.arch = PIC;

    exts.bin = ".cof";
    exts.obj = ".o";
    exts.lib = ".a";

    //  var_set("TARGET", cfg.mach.bits == _14 ? "pic16" : "pic18");

    if(cfg.chip.len == 0)
      stralloc_copys(&cfg.chip, "16f876a");

    stralloc_nul(&cfg.chip);
    var_set("CHIP", cfg.chip.s);

    /* {
       stralloc chipdef;
       stralloc_init(&chipdef);
       stralloc_copys(&chipdef, "-DPIC");
       stralloc_cat(&chipdef, &cfg.chip);
       stralloc_upper(&chipdef);
       stralloc_cats(&chipdef, "=1");
       var_push_sa("CPPFLAGS", &chipdef);
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

    //    var_push("LDFLAGS", "--out-fmt-ihx");

    if(cfg.mach.bits == _16) {
      var_push("LIBS", "-llibm18f.lib");
    } else {
      var_push("LIBS", "-llibm.lib");
    }

    set_command(&lib_command, "$(LIB) rcs $@", "$^");
    stralloc_copys(&compile_command, "$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(DEFS) -c $< -o $@");
    stralloc_copys(
        &link_command,
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
    // var_push("CPPFLAGS", cfg.mach.bits == _14 ? "-DPIC16=1" : "-DPIC18=1");

    var_set("CC", cfg.mach.bits == _14 ? "picc" : "picc18");
    var_set("LINK", cfg.mach.bits == _14 ? "picc" : "picc18");

    if(cfg.chip.len == 0)
      stralloc_copys(&cfg.chip, "16f876a");

    stralloc_nul(&cfg.chip);
    var_set("CHIP", cfg.chip.s);

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

    /*var_push("CFLAGS", "--errformat=\"%f:%l:%c error [%n]: %s\"");
    var_push("CFLAGS", "--warnformat=\"%f:%l:%c warning [%n]: %s\"");*/

    var_push("LDFLAGS", "--asmlist");
    //    var_push("CPPFLAGS", "-D__$(CHIP)=1");

    set_command(&lib_command, "$(LIB) $@", "$^");
    stralloc_copys(&compile_command, "$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CPPFLAGS) $(DEFS) --pass1 -c $< -o$@");
    stralloc_copys(
        &link_command,
        "$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(LDFLAGS) $(EXTRA_LDFLAGS) -o$@ $^ $(LIBS) $(EXTRA_LIBS) $(STDC_LIBS)");

  } else if(str_start(compiler, "xc8") || str_start(compiler, "picc")) {

    //    no_libs = 1;
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

    stralloc_nul(&cfg.chip);
    var_set("CHIP", cfg.chip.s);

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

    // var_push("CFLAGS", "-fp:precise");

    var_push("CFLAGS", "-q");
    var_push("CFLAGS", "--asmlist");
    var_push("CFLAGS", "--chip=$(CHIP)");
    var_push("CPPFLAGS", "-D__$(CHIP)=1");

    var_push("LDFLAGS", "--output=-elf,+mcof");
    //    var_push("LDFLAGS", "--output=-mcof,+elf:multilocs");
    var_push("LDFLAGS", "--summary=default");

    var_push("LDFLAGS", "--runtime=default,+clear,+init,-keep,-no_startup,-osccal,-resetbits,+download,+clib");
    // var_push("LDFLAGS", "--output=-default,elf,+mcof");
    // var_push("LDFLAGS", "--output=-mcof,+elf");
    var_push("LDFLAGS", "--stack=compiled");

    /*var_push("CFLAGS", "--errformat=\"%f:%l:%c error [%n]: %s\"");
    var_push("CFLAGS", "--warnformat=\"%f:%l:%c warning [%n]: %s\"");*/

    stralloc_copys(&preprocess_command, "$(CPP) $(CPPFLAGS) $(DEFS) $< -o$@");
    stralloc_copys(&compile_command,
                   "$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(EXTRA_C-FLAGS) $(CPPFLAGS) $(DEFS) --pass1 -c $< -o$@");
    stralloc_copys(
        &link_command,
        "$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(LDFLAGS) $(EXTRA_LDFLAGS) -o$@ $^ $(LIBS) $(EXTRA_LIBS) $(STDC_LIBS)");
    stralloc_copys(
        &lib_command,
        "$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(LDFLAGS) $(EXTRA_LDFLAGS) --OUTPUT=lpp --memorysummary -G -m$@.map -P "
        "--asmlist "
        "--output=default,-inhx032 --output=-mcof,+elf:multilocs -o$@.elf $^ $(LIBS) $(EXTRA_LIBS) $(STDC_LIBS)");

  } else {
    return 0;
  }

  if(cfg.build_type == BUILD_TYPE_DEBUG) {
    var_push("DEFS", "-D_DEBUG=1");
  } else {
    var_push("DEFS", "-DNDEBUG=1");
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
        push_lib("EXTRA_LIBS", "wsock32");
      else
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
      strlist* cross = var_set("CROSS_COMPILE", str_end(tools.toolchain, "64") ? "x86_64" : "i686");

      //      cross->sep = '-';
      stralloc_cats(&cross->sa, str_start(tools.toolchain, "mingw") ? "-w64-" : "-pc-");
      stralloc_cats(&cross->sa,
                    str_start(tools.toolchain, "mingw") ? "mingw32"
                                                        : str_start(tools.toolchain, "msys") ? "msys" : "cygwin");

      stralloc_catc(&cross->sa, '-');
    }
  }

  if(var_isset("CROSS_COMPILE")) {
    stralloc_prepends(&compile_command, "$(CROSS_COMPILE)");
    stralloc_prepends(&lib_command, "$(CROSS_COMPILE)");
    stralloc_prepends(&link_command, "$(CROSS_COMPILE)");
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
  const char *outfile = NULL, *infile = NULL, *dir = NULL;
  set_t toks;
  strarray args;
  strlist cmdline;
  static strarray libs, includes;
  buffer filebuf;
  buffer* out = buffer_1;
  char** it;
  const char* s;
  size_t n;
  target *rule, *all, *compile;
  char** arg;
  char **ptr, *x;
  MAP_PAIR_T t;
  strarray sources;
  strarray_init(&sources);

#if !WINDOWS_NATIVE
  sig_ignore(SIGTRAP);
#endif

  struct longopt opts[] = {{"help", 0, NULL, 'h'},
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
                           /*                           {"install-bins", 0, &inst_bins, 1},
                                                     {"install-libs", 0, &inst_libs, 1},*/
                           {"builddir", 1, 0, 'd'},
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
                           {0, 0, 0, 0}};

  errmsg_iam(argv[0]);
  uint32_seed(NULL, 0);
#ifdef _MSC_VER
  unix_optbuf = buffer_1;
#endif

  byte_zero(&cfg, sizeof(cfg));
  byte_zero(&dirs, sizeof(dirs));
  /*byte_zero(&sourcedirs, sizeof(sourcedirs));
  byte_zero(&targetdirs, sizeof(targetdirs));*/
  byte_zero(&rules, sizeof(rules));
  byte_zero(&vars, sizeof(vars));
  byte_zero(&srcs, sizeof(srcs));
  byte_zero(&tools, sizeof(tools));

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

  strlist_init(&vpath, ' ');

  strlist_init(&cmdline, ' ');
  strlist_fromv(&cmdline, (const char**)argv, argc);

  for(;;) {
    c = unix_getopt_long(argc, argv, "ho:O:B:L:d:t:m:n:a:D:l:I:c:s:p:P:S:if:C", opts, &index);
    if(c == -1)
      break;
    if(c == 0)
      continue;

    switch(c) {
      case 'h':
        usage(argv[0]);
        ret = 0;
        goto quit;
      case 'C': cfg.lang = LANG_C; break;
      case 'c': cross_compile = unix_optarg; break;
      case 'o': outfile = unix_optarg; break;
      case 'O': exts.obj = unix_optarg; break;
      case 'B': exts.bin = unix_optarg; break;
      case 'S': strlist_push(&build_as_lib, unix_optarg ? unix_optarg : argv[unix_optind]); break;
      case 'X': exts.lib = unix_optarg; break;
      case 'd': dir = unix_optarg; break;
      case 't': tools.toolchain = tools.compiler = unix_optarg; break;
      case 'm': tools.make = unix_optarg; break;
      case 'P': tools.preproc = unix_optarg; break;
      case 'a': set_machine(unix_optarg); break;
      case 's': set_system(unix_optarg); break;
      case 'n': stralloc_copys(&output_name, unix_optarg); break;
      case 'p':
        if(unix_optarg)
          set_chip(unix_optarg);
        break;
      case 'f': infile = unix_optarg; break;
      case 'l': strarray_push(&libs, unix_optarg); break;
      case 'I': {
        buffer_puts(buffer_2, "Add -I: ");
        buffer_puts(buffer_2, unix_optarg);
        buffer_putnlflush(buffer_2);
        strarray_push(&includes, unix_optarg);
        break;
      }
      case 'i':
        inst_bins = 1;
        inst_libs = 1;
        break;
      case 'D': push_define(unix_optarg); break;

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
  if(inst_libs)
    cmd_libs = 1;

  if(!format_linklib_fn)
    format_linklib_fn = &format_linklib_lib;

  strlist_init(&dirs.this, pathsep_make);
  strlist_init(&dirs.out, pathsep_make);
  strlist_init(&dirs.build, pathsep_make);
  strlist_init(&dirs.work, pathsep_make);

  if(outfile) {
    int fd;
    path_dirname(outfile, &dirs.out.sa);
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
    //  path_absolute_sa(&dirs.out.sa);
  }

  path_getcwd(&dirs.this.sa);

  if(cfg.build_type == -1) {
    if((cfg.build_type = extract_build_type(&dirs.build.sa)) == -1)
      if((cfg.build_type = extract_build_type(&dirs.this.sa)) == -1)
        cfg.build_type = extract_build_type(&dirs.out.sa);
  }
  if(cfg.build_type == -1)
    cfg.build_type = BUILD_TYPE_DEBUG;

  if(tools.make == NULL && tools.compiler) {
    if(str_start(tools.compiler, "b"))
      tools.make = "borland";
    else if(str_start(tools.compiler, "msvc"))
      tools.make = "nmake";
    else if(str_start(tools.compiler, "g"))
      tools.make = "gmake";
    else if(str_start(tools.compiler, "o"))
      tools.make = "omake";
    else if(str_start(tools.compiler, "po"))
      tools.make = "pomake";
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
      stralloc_prepends(&var_list("CC")->sa, "$(CROSS_COMPILE)");
    if(var_isset("CXX"))
      stralloc_prepends(&var_list("CXX")->sa, "$(CROSS_COMPILE)");
    if(var_isset("AR"))
      stralloc_prepends(&var_list("AR")->sa, "$(CROSS_COMPILE)");
  }

  batchmode = batch && stralloc_contains(&compile_command, "-Fo");

  if(batch)
    pathsep_args = pathsep_make;

  strarray_foreach(&libs, it) { with_lib(*it); }
  strarray_foreach(&includes, it) { includes_add(*it); }

  stralloc_replacec(&dirs.out.sa, PATHSEP_C == '/' ? '\\' : '/', PATHSEP_C);
  //  path_absolute_sa(&dirs.out.sa);

  stralloc_nul(&dirs.out.sa);
  stralloc_nul(&dirs.this.sa);

  if(dirs.build.sa.len == 0) {
    if(strlist_contains(&dirs.out, "build")) {
      stralloc_copy(&dirs.build.sa, &dirs.out.sa);
      // path_relative(dirs.out.sa.s, dirs.this.sa.s, &dirs.build.sa);
    } else if(tools.toolchain && !strlist_contains(&dirs.this, "build")) {
      stralloc target;
      stralloc_init(&target);
      stralloc_copys(&target, tools.toolchain);

      if(cfg.chip.s) {
        stralloc_cats(&target, "-");
        stralloc_cat(&target, &cfg.chip);
      }
      stralloc_nul(&target);

      stralloc_copy(&dirs.build.sa, &dirs.out.sa);
      strlist_push(&dirs.build, dir ? dir : "build");
      strlist_push_sa(&dirs.build, &target);
      strlist_push(&dirs.build, build_types[cfg.build_type]);

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

  stralloc_nul(&dirs.this.sa);
  stralloc_nul(&dirs.out.sa);
  stralloc_nul(&dirs.build.sa);
  stralloc_nul(&dirs.work.sa);

  // debug_sa("dirs.this", &dirs.this.sa);
  // debug_sa("dirs.out", &dirs.out.sa);
  // debug_sa("dirs.build", &dirs.build.sa);

  if(tools.preproc)
    var_set("CPP", tools.preproc);

  includes_cppflags();

  /*

    stralloc_nul(&dirs.out.sa);
    stralloc_nul(&dirs.this.sa);
    path_relative(dirs.out.sa.s, dirs.this.sa.s, &dirs.out.sa);
  */

  // debug_sa("dirs.work", &dirs.work.sa);

  stralloc_nul(&dirs.this.sa);
  stralloc_nul(&dirs.out.sa);
  path_relative(dirs.this.sa.s, dirs.out.sa.s, &srcdir);
  stralloc_nul(&srcdir);
  // debug_sa("srcdir", &srcdir);

  if(dirs.out.sa.len) {
    stralloc_replacec(&dirs.this.sa, PATHSEP_C == '/' ? '\\' : '/', PATHSEP_C);
    stralloc_replacec(&dirs.out.sa, PATHSEP_C == '/' ? '\\' : '/', PATHSEP_C);

    // debug_sa("dirs.this", &dirs.this.sa);
    // debug_sa("dirs.out", &dirs.out.sa);

    path_absolute_sa(&dirs.out.sa);
    stralloc_zero(&tmp);
    path_relative(dirs.this.sa.s, dirs.out.sa.s, &tmp);

    // if(tmp.len) {
    stralloc_copy(&srcdir, &tmp);
    // debug_sa("srcdir", &srcdir);
    //}
    stralloc_zero(&tmp);
  }

  // debug_sa("srcdir", &srcdir);

  path_relative(dirs.build.sa.s, dirs.out.sa.s, &tmp);
  // debug_sa("tmp", &tmp);

  if(dirs.build.sa.len > dirs.out.sa.len && byte_equal(dirs.out.sa.s, dirs.out.sa.len, dirs.build.sa.s)) {
    const char* x = dirs.build.sa.s + dirs.out.sa.len;
    size_t n = dirs.build.sa.len - dirs.out.sa.len;
    stralloc_zero(&dirs.work.sa);
    if(n > 0 && *x == '/') {
      x++;
      n--;
    }
    stralloc_catb(&dirs.work.sa, x, n);
  }

  //  path_relative(dirs.build.sa.s, dirs.out.sa.s, &dirs.work.sa);
  stralloc_nul(&dirs.work.sa);

  stralloc_replacec(&dirs.work.sa, pathsep_make == '/' ? '\\' : '/', pathsep_make);

  mkdir_components(&dirs.out, 0755);
  mkdir_components(&dirs.build, 0755);
  if(stralloc_diffs(&dirs.work.sa, "."))
    mkdir_components(&dirs.work, 0755);
  /*

    if(tmp.len) {
      stralloc_catc(&tmp, pathsep_make);
      stralloc_copy(&dirs.build.sa, &tmp);
    }
    stralloc_free(&tmp);

    //debug_sa("dirs.build", &dirs.build.sa);
  */

  if(str_equal(tools.make, "gmake")) {
    stralloc_nul(&dirs.work.sa);
    strlist_push_unique(&vpath, ".");
    strlist_push_unique_sa(&vpath, &dirs.work.sa);
  }
  strarray_init(&args);
  strarray_init(&srcs);

  if(infile) {
    const char* x;
    size_t n;

    path_dirname(infile, &dirs.this.sa);
    stralloc_nul(&dirs.this.sa);
    sources_get(dirs.this.sa.s);

    if((x = mmap_read(infile, &n))) {
      while(n > 0) {
        size_t i = byte_chr(x, n, '\n');

        if(!(i > 2 && x[0] == '-' && x[1] == '-')) {

          if(i > 0) {
            input_command_line(x, i);
          }
        }

        if(i < n)
          i++;

        x += i;
        n -= i;
      }

      mmap_unmap(x, n);
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

#if WINDOWS_NATIVE && !MINGW
    if(str_rchrs(argv[unix_optind], "*?", 2) < str_len(argv[unix_optind]))
      strarray_glob(&args, arg.s);
    else
#endif

      strarray_push(&args, arg.s);
    ++unix_optind;
  }

  /* No arguments given */
  if(strarray_size(&args) == 0 && !infile) {

    buffer_putsflush(buffer_2, "ERROR: No arguments given\n\n");
    usage(argv[0]);
    ret = 1;
    goto quit;
  }

  all = rule_get("all");

  if(strlist_count(&dirs.work) && !stralloc_equals(&dirs.work.sa, ".")) {

    if((rule = rule_get_sa(&dirs.work.sa))) {
      stralloc_weak(&rule->recipe, &mkdir_command);

      add_path_sa(&all->prereq, &dirs.work.sa);
    }
  }

  //  strarray_dump(buffer_2, &args);

  strarray_foreach(&args, arg) {

    if(!path_exists(*arg)) {
      buffer_putm_internal(buffer_2, "ERROR: Doesn't exist: ", *arg, newline, NULL);
      buffer_flush(buffer_2);
      ret = 127;
      goto fail;
    }

    if(is_source(*arg) || is_include(*arg))
      sources_add(*arg);
    else if(path_is_directory(*arg))
      sources_get(*arg);
  }
  {
    set_iterator_t it;

    set_foreach(&srcs, it, x, n) { strarray_pushb(&sources, x, n); }
    strarray_sort(&sources, &sources_sort);
  }

#ifdef DEBUG_OUTPUT_
  buffer_puts(buffer_2, "strarray sources:");
  strarray_dump(buffer_2, &sources);
  buffer_putnlflush(buffer_2);
#endif

  if(str_end(tools.make, "make")) {
    stralloc rulename;
    stralloc_init(&rulename);

    if(str_start(tools.make, "g")) {
      stralloc_copy(&rulename, &dirs.work.sa);
      stralloc_cats(&rulename, "/%");
      stralloc_cats(&rulename, exts.obj);
      stralloc_cats(&rulename, ": %");
      stralloc_cats(&rulename, /* tools.preproc ? exts.pps : */ ".c");
    } else {
      stralloc_copys(&rulename, ".c");
      stralloc_cats(&rulename, exts.obj);
    }

    compile = rule_get_sa(&rulename);
    stralloc_weak(&compile->recipe, &compile_command);

    stralloc_free(&rulename);
  }

  if(str_start(tools.make, "g")) {
    stralloc bdir;

    stralloc_init(&bdir);
    path_relative(dirs.build.sa.s, dirs.work.sa.s, &bdir);
    stralloc_nul(&bdir);
    stralloc_cats(&dirs.work.sa, PATHSEP_S);

    var_set("BUILDDIR", dirs.work.sa.s);

    stralloc_copys(&dirs.build.sa, "$(BUILDDIR)");
  }

  if(((batch | shell) && stralloc_equals(&dirs.work.sa, ".")))
    batchmode = 1;

  if(!infile) {

    stralloc src;
    stralloc_init(&src);

    strarray_foreach(&sources, ptr) { sourcedir_addsource(*ptr, &sources); }
    sourcedir_populate(&sources);
    strarray_free(&sources);

    stralloc_free(&src);
#if DEBUG_OUTPUT_

    // debug_stra("srcs", &srcs);
    buffer_puts(buffer_2, "targetdirs:\n");

    MAP_FOREACH(targetdirs, t) {
      uint32* count_ptr = (uint32*)MAP_VALUE(t);
      buffer_puts(buffer_2, "  '");
      buffer_puts(buffer_2, MAP_KEY(t));
      buffer_puts(buffer_2, "' => ");
      buffer_putulong(buffer_2, *count_ptr);
      buffer_putnlflush(buffer_2);
    }
    buffer_putnlflush(buffer_2);
#endif

#if DEBUG_OUTPUT_

    sourcedir_dump_all(buffer_2);
#endif

#if DEBUG_OUTPUT_

    // debug_stra("srcs", &srcs);
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
      uint32* count_ptr = (uint32*)MAP_VALUE(t);
      buffer_puts(buffer_2, "  '");
      buffer_puts(buffer_2, MAP_KEY(t));
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
        sourcedir* srcdir = *(sourcedir**)MAP_VALUE(t);

        /*if(tools.preproc) {
          gen_simple_compile_rules(rules, srcdir, MAP_KEY(t), ".c", exts.pps, &preprocess_command);
          gen_simple_compile_rules(rules, srcdir, MAP_KEY(t), exts.pps, exts.obj, &compile_command);
        } else */
        { gen_simple_compile_rules(srcdir, MAP_KEY(t), ".c", exts.obj, &compile_command); }
      }
    }

    if(cmd_bins) {

      cmd_bins = gen_link_rules();
    }

    if(cmd_bins == 0 || cmd_libs == 1) {
      MAP_PAIR_T t;
      MAP_FOREACH(rules, t) {
        target* tgt = MAP_VALUE(t);

        if(stralloc_equal(&tgt->recipe, &lib_command) && cmd_libs)
          set_adds(&all->prereq, MAP_KEY(t));
      }
    }
  }

  gen_clean_rule();

  {
    MAP_PAIR_T t;
    MAP_FOREACH(rules, t) {
      target* tgt = MAP_VALUE(t);
      // print_rule_deps(buffer_2, tgt);
    }
  }
  if(inst_bins || inst_libs)
    gen_install_rules();

  {
    MAP_PAIR_T t;
    MAP_FOREACH(sourcedirs, t) {
      sourcedir* srcdir = *(sourcedir**)MAP_VALUE(t);

      //  strlist_sort(&srcdir->pptoks, (strlist_cmpfn_t*)&case_diffs);
      // debug_sl(t->key, &srcdir->pptoks, ", ");
    }
  }

fail:

  if(!case_diffs(tools.make, "mplab")) {

    output_mplab_project(out, 0, 0, &include_dirs);
    goto quit;
  }

  buffer_putm_internal(out, comment, " Generated by:", newline, comment, "  ", NULL);
  buffer_putsa(out, &cmdline.sa);
  buffer_putsflush(out, newline);

  {
    strlist varnames;
    strlist_init(&varnames, '\0');

    get_keys(&vars, &varnames);
    /*    buffer_puts(buffer_2, "varnames: ");
       strlist_dump(buffer_2, &varnames); */

    output_all_vars(out, &vars, &varnames);
  }

  if(str_equal(tools.make, "gmake")) {
    stralloc_nul(&vpath.sa);

    //      buffer_putm_internal(out, "\nvpath ", vpath.sa.s, "\n", NULL);

    stralloc_replacec(&vpath.sa, ' ', ':');
    buffer_putm_internal(out, "VPATH = ", vpath.sa.s, "\n\n", NULL);
    buffer_flush(out);
  }

  if(ninja) {
    output_build_rules(out, "cc", &compile_command);
    output_build_rules(out, "link", &link_command);
    output_build_rules(out, "lib", &lib_command);
    put_newline(out, 0);
  }

  if(batch || shell) {
    if(batch) {
      buffer_putm_internal(out, "CD %~dp0", newline, NULL);
    } else {
      buffer_putm_internal(out, "cd \"$(dirname \"$0\")\"\n\n", NULL);
    }

    output_script(out, NULL);
  } else
    output_all_rules(out);

quit :

{
  strlist deps;
  strlist_init(&deps, '\0');

  MAP_PAIR_T t;
  MAP_FOREACH(sourcedirs, t) {
    sourcedir* sdir = *(sourcedir**)MAP_VALUE(t);

    if(1 /* && set_size(&sdir->deps)*/) {
      strlist_zero(&deps);
      sourcedir_deps(sdir, &deps);
#ifdef DEBUG_OUTPUT_
      buffer_putm_internal(buffer_2, "source directory '", MAP_KEY(t), "' deps =\n", NULL);
      strlist_dump(buffer_2, &deps);
      buffer_putnlflush(buffer_2);
#endif
    }
  }
}

  {
    sourcefile* source;
    slink* link;
    strlist deps;
    strlist_init(&deps, '\0');
    slist_foreach(sourcelist, link) {
      source = slist_data(link);

      if(0 && /*source->name[str_find(source->name, "genmakefile")] && strlist_count(&source->deps)*/ 1) {
        buffer_putm_internal(buffer_2, "source: ", source->name, " deps: ", NULL);
        strlist_zero(&deps);
        sources_deps(source, &deps);
        buffer_puts(buffer_2, " includes: ");

        strlist_dump(buffer_2, &source->includes);
        buffer_putnlflush(buffer_2);
      }
    }
  }
  //  MAP_DESTROY(sourcedirs);
  return ret;
}
