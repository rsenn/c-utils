#include "../byte.h"
#include "../stralloc.h"
#include "../utf8.h"

/* stralloc_catb copies the string buf[0], buf[1], ..., buf[len - 1] into
 * sa, allocating space if necessary, and returns 1. If it runs out of
 * memory, stralloc_catb leaves sa alone and returns 0. */
int
stralloc_catwcb(stralloc* sa, const wchar_t* x, size_t n) {
  size_t bytes = 0;

  for(size_t i = 0; x[i]; ++i)
    bytes += wc_u8len(x[i]);

  if(stralloc_readyplus(sa, bytes)) {
    wcs_to_u8s(stralloc_end(sa), x, n);
    sa->len += bytes;
    return 1;
  }

  return 0;
}
