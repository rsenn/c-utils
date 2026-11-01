#include "../byte.h"
#include "../stralloc.h"
#include "../utf8.h"

/* stralloc_copyb copies the string buf[0], buf[1], ..., buf[len - 1] into
 * sa, allocating space if necessary, and returns 1. If it runs out of
 * memory, stralloc_copyb leaves sa alone and returns 0. */
int
stralloc_copywcb(stralloc* sa, const wchar_t* x, size_t n) {
  size_t i;
  size_t bytes = 0;

  /* only the first n characters count (it used to measure up to the terminator) */
  for(i = 0; i < n && x[i]; ++i)
    bytes += wc_u8len(x[i]);

  if(stralloc_ready(sa, bytes)) {
    sa->len = bytes;
    wcs_to_u8s(sa->s, x, bytes); /* the count is in bytes */
    return 1;
  }

  return 0;
}
