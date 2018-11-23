#include "../fmt.h"
#include "../stralloc.h"

unsigned int
stralloc_cathexb(stralloc* sa, const void* d, unsigned int n) {
  unsigned int len;
  len = n << 1;
  if(!stralloc_readyplus(sa, len))
    return 0;
  fmt_hexb(sa->s + sa->len, d, n);
  sa->len += len;
  return 1;
}
