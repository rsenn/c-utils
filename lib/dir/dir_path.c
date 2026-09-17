#include "../stralloc.h"
#include "../dir_internal.h"

void
dir_path(struct dir_s* d, stralloc* sa) {
#if USE_READDIR
  /* dir_open() stashes the path it was opened with in dir_path (see
   * dir_open.c) -- the same field dir_time.c already reads for this
   * exact purpose. DIR has no portable way to recover the path it was
   * opened with (glibc's __d_dirname is a private implementation
   * detail, not part of any POSIX interface). */
  stralloc_copys(sa, (const char*)dir_INTERNAL(d)->dir_path);
#else
#if USE_WIDECHAR
  stralloc_copywcs(sa, dir_INTERNAL(d)->dir_path);
#else
  stralloc_copys(sa, dir_INTERNAL(d)->dir_path);
#endif

#endif

  if(sa->len > 0 && sa->s[sa->len - 1] == '*')
    --sa->len;

#if USE_READDIR
  /* unlike the Windows branch's dir_path (which always ends in "\*",
   * trimmed to a trailing "\" above), the raw path dir_open() was
   * given has no guaranteed trailing separator -- add one, matching
   * dir_time.c's identical path-plus-name join. */
  if(sa->len > 0)
    stralloc_catc(sa, '/');
#endif

#if !USE_READDIR && USE_WIDECHAR
  stralloc_catwcs(sa, dir_NAME(d));
#else
  stralloc_cats(sa, dir_NAME(d));
#endif
}
