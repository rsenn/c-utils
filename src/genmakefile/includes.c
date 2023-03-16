#include "includes.h"
#include "rule.h"
#include "var.h"
#include "path.h"
#include "../../lib/mmap.h"
#include "../../lib/str.h"
#include "../../lib/strlist.h"
#include "../../lib/path.h"

strlist include_dirs = {0};

void extract_includes(const char*, size_t, strlist*, int);

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
includes_get(const char* srcfile, strlist* includes, int sys, stralloc* thisdir, char pathsep_make) {
  const char* x;
  size_t n;
  if((x = path_mmap_read(srcfile, &n, thisdir, pathsep_make))) {
    extract_includes(x, n, includes, sys);
    mmap_unmap(x, n);
    return 1;
  }
  return 0;
}

void
includes_add_b(const char* dir, size_t len, stralloc* builddir, stralloc* outdir) {
  static stralloc abs;
  stralloc_zero(&abs);
  path_normalize_b(dir, len, &abs, builddir, outdir);
  if(strlist_push_unique_sa(&include_dirs, &abs)) {
#ifdef DEBUG_OUTPUT
    buffer_puts(buffer_2, "Added to include dirs: ");
    buffer_putsa(buffer_2, &abs);
    buffer_putnlflush(buffer_2);
#endif
  }
}

void
includes_add(const char* dir, stralloc* builddir, stralloc* outdir) {
  includes_add_b(dir, str_len(dir), builddir, outdir);
}

/**
 * @brief includes_to_libs  Include list
 * to library list
 * @param includes
 * @param libs
 */
void
includes_to_libs(const set_t* includes,
                 strlist* libs,
                 const char* libpfx,
                 stralloc* thisdir,
                 const char* incext,
                 const char* libext) {
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
    path_concatb(thisdir->s, thisdir->len, sa.s, sa.len, &sa);
    // debug_sa("include", &sa);
    stralloc_zero(&lib);
    stralloc_copys(&lib, path_basename(sa.s));
    if(stralloc_endb(&lib, incext, 2))
      lib.len -= 2;
    stralloc_cats(&lib, libext);
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
