#include "../stralloc.h"
#include "../dir_internal.h"

void
dir_path(struct dir_s* d, stralloc* sa) {
#if USE_READDIR
  DIR* dh = dir_INTERNAL(d)->dir_handle;

  //stralloc_copys(sa, dh->__d_dirname);
#else
#if USE_WIDECHAR
  stralloc_copywcs(sa, dir_INTERNAL(d)->dir_path);
#else
  stralloc_copys(sa, dir_INTERNAL(d)->dir_path);
#endif

#endif
  if(sa->len > 0 && sa->s[sa->len - 1] == '*') --sa->len;

#if !USE_READDIR && USE_WIDECHAR
  stralloc_catwcs(sa, dir_NAME(d));
#else
  stralloc_cats(sa, dir_NAME(d));
#endif
}
