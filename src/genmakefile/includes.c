#include "includes.h"
#include "rule.h"
#include "var.h"
#include "path.h"
#include "../../lib/mmap.h"
#include "../../lib/str.h"
#include "../../lib/strlist.h"
#include "../../lib/path.h"
#include "../../debug.h"
#include "../../genmakefile.h"

strlist include_dirs = {0};

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

void
includes_cppflags(void) {
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
includes_get(const char* srcfile, strlist* includes, int sys, char pathsep_make) {
  const char* x;
  size_t n;

  if((x = path_mmap_read(srcfile, &n, pathsep_make))) {
    includes_extract(x, n, includes, sys);
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

/**
 * @brief includes_to_libs  Include list
 * to library list
 * @param includes
 * @param libs
 */
void
includes_to_libs(const set_t* includes, strlist* libs, const char* libpfx, const char* incext, const char* libext) {
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

    if(stralloc_endb(&lib, incext, 2))
      lib.len -= 2;

    stralloc_cats(&lib, libext);

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
