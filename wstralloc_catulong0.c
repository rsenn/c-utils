#include "wstralloc.h"
#include "fmt.h"

int wstralloc_catulong0(wstralloc* sa, unsigned long int in, size_t n) {
  if(wstralloc_readyplus(sa, fmt_ulong0(0, in, n))) {
    sa->len += fmt_ulong0(sa->s + sa->len, in, n);
    return 1;
  } else
    return 0;
}
