#include "../stralloc.h"
#include "../fmt.h"

unsigned int stralloc_cathexb(register stralloc *sa, const void *d, register unsigned int n)
{
  register unsigned int len;
  len = n << 1;
  if(!stralloc_readyplus(sa, len)) return 0;
  fmt_hexb(sa->s + sa->len, d, n);
  sa->len += len;
  return 1;
}
