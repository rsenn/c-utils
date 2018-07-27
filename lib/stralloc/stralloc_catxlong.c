#include "../stralloc.h"
#include "../fmt.h"

unsigned int stralloc_catxlong(stralloc *sa, unsigned long u) {
  unsigned int i;
  i = fmt_xlong(FMT_LEN, u);
  if(!stralloc_readyplus(sa, i)) return 0;
  fmt_xlong(sa->s + sa->len, u);
  sa->len += i;
  return 1;
}
