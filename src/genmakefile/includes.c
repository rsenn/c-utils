#include "includes.h"
#include "rule.h"
#include "ansi.h"
#include "../../debug.h"
#include "../../genmakefile.h"

strlist include_dirs = {0, 0, 0};

/**
 * @brief      Extract #include directives
 *
 * @param      x         Input buffer
 * @param      n         Input length
 * @param      includes  Output list
 * @param      sys       Include system includes
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
 * @brief      Puts include directories into CPPFLAGS
 */
void
includes_cppflags(void) {
  const char* dir;
  stralloc arg;

  stralloc_init(&arg);

  strlist_foreach_s(&include_dirs, dir) {
    stralloc_zero(&arg);
    stralloc_cats(&arg, dir);
    // path_relative_to(dir, dirs.this.sa.s, &arg);

#ifdef DEBUG_OUTPUT
    buffer_putm_internal(debug_buf, "[1]", PINK256, "includes_cppflags", NC, " include_dir=", 0);
    buffer_putsa(debug_buf, &arg);
    buffer_putnlflush(debug_buf);
#endif

    stralloc_prepends(&arg, "-I");
    var_push_sa("CPPFLAGS", &arg);
  }

  stralloc_free(&arg);
}

/**
 * @brief      Extract includes from file
 *
 * @param      srcfile   Source file
 * @param      includes  Include file list
 * @param      sys       System includes
 * @param[in]  psm       Path separator character
 *
 * @return     1 on success
 */
int
includes_get(const char* srcfile, strlist* includes, int sys, char psm) {
  const char* x;
  size_t n;

  if((x = path_mmap_read(srcfile, &n, psm))) {
    includes_extract(x, n, includes, sys);
    mmap_unmap(x, n);
    return 1;
  }

  return 0;
}

/**
 * @brief      Add include path
 *
 * @param[in]  dir   Directory string
 * @param[in]  len   Directory length
 */
void
includes_add_b(const char* dir, size_t len) {
  stralloc d, tmp, to;

  stralloc_init(&d);
  stralloc_init(&tmp);
  stralloc_init(&to);

  path_normalize_sa_b(dir, len, &d);
  path_absolute(dirs.work.sa.s, &to);
  path_relative_to_b(d.s, d.len, to.s, to.len, &tmp);
  stralloc_free(&d);

  if(strlist_push_unique_sa(&include_dirs, &tmp)) {
#ifdef DEBUG_OUTPUT
    buffer_putm_internal(debug_buf, "[1]", PINK256, "includes_add_b", NC, " tmp=", 0);
    buffer_putsa(debug_buf, &tmp);
    buffer_putnlflush(debug_buf);
#endif
  }

  stralloc_free(&tmp);
  stralloc_free(&to);
}

/**
 * @brief      Add include path
 *
 * @param[in]  dir   Directory string
 */
void
includes_add(const char* dir) {
  includes_add_b(dir, str_len(dir));
}

/**
 * @brief      Include list to library list
 *
 * @param      includes  The includes
 * @param      libs      The libs
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

    if(!(n > str_len(libpfx) && byte_equal(s, str_len(libpfx), libpfx)) && byte_chr(s, n, PATHSEP_C) == n)
      path_concatb(libpfx, str_len(libpfx), sa.s, sa.len, &sa);

    path_concatb(dirs.this.sa.s, dirs.this.sa.len, sa.s, sa.len, &sa);

    stralloc_zero(&lib);
    stralloc_copys(&lib, path_basename(sa.s));

    if(stralloc_endb(&lib, exts.inc, 2))
      lib.len -= 2;

    stralloc_cats(&lib, exts.lib);

    if((rule = rule_find_sa(&lib))) {
#ifdef DEBUG_OUTPUT
      debug_str("lib", rule->name);
#endif

      strlist_push(libs, rule->name);
    }
  }

  stralloc_free(&lib);
  stralloc_free(&sa);
}

/**
 * @brief      Find a file in include directories
 *
 * @param[in]  x     File name buffer
 * @param[in]  n     File name length
 * @param      out   Output stralloc
 *
 * @return     1 if found, 0 otherwise
 */
int
includes_find_sa(const char* x, size_t n, stralloc* out) {
  const char* dir;

  strlist_foreach_s(&include_dirs, dir) {
    stralloc_copys(out, dir);
    stralloc_catc(out, PATHSEP_C);
    stralloc_catb(out, x, n);
    stralloc_nul(out);

#ifdef DEBUG_OUTPUT_
    buffer_puts(debug_buf, __func__);
    buffer_puts(debug_buf, ": ");
    buffer_putsa(debug_buf, out);
    buffer_putnlflush(debug_buf);
#endif

    if(path_exists(out->s))
      return 1;
  }

  return 0;
}

/**
 * @brief      Find file in include path
 *
 * @param[in]  x     File name buffer
 * @param[in]  n     File name length
 *
 * @return     Path of found file or NULL
 */
char*
includes_find_b(const char* x, size_t n) {
  stralloc path;
  stralloc_init(&path);

  if(includes_find_sa(x, n, &path))
    return path.s;

  stralloc_free(&path);
  return 0;
}

/**
 * @brief      Find file in include path
 *
 * @param[in]  s     File name
 *
 * @return     Path of found file or NULL
 */
char*
includes_find(const char* s) {
  return includes_find_b(s, str_len(s));
}
