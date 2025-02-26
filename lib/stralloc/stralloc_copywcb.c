#include "../byte.h"
#include "../stralloc.h"
#include "../utf8.h"

/* stralloc_copyb copies the string buf[0], buf[1], ..., buf[len - 1] into
 * sa, allocating space if necessary, and returns 1. If it runs out of
 * memory, stralloc_copyb leaves sa alone and returns 0. */
int
stralloc_copywcb(stralloc* sa, const wchar_t* buf, size_t len) {
  size_t i, n = 0;

  for(i = 0; buf[i]; ++i)
    n += wc_u8len(buf[i]);

  if(stralloc_ready(sa, n)) {
    sa->len = n;
    wcs_to_u8s(sa->s, buf, len);
    return 1;
  }
  return 0;
}
