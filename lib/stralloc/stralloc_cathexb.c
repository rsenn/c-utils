#include "../fmt.h"
#include "../stralloc.h"

size_t
stralloc_cathexb(stralloc* sa, const void* d, unsigned int n) {
  size_t len;
  len = n << 1;
  if(!stralloc_readyplus(sa, len))
    return 0;
  fmt_hexb(sa->s + sa->len, d, n);
  sa->len += len;
  return 1;
}
