#include "../fmt.h"
#include "../stralloc.h"

size_t
stralloc_catxlong0(stralloc* sa, unsigned long n, unsigned int pad) {
  unsigned int i, j;
  i = fmt_xlong(FMT_LEN, n);
  if(!stralloc_readyplus(sa, i > pad ? i : pad))
    return 0;
  pad = i < pad ? pad - i : 0;
  for(j = 0; j < pad; j++) sa->s[sa->len + j] = '0';

  fmt_xlong(sa->s + sa->len + j, n);
  sa->len += j + i;
  return 1;
}
