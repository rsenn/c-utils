#include "../byte.h"
#include "../stralloc.h"

/* stralloc_catb adds the string buf[0], buf[1], ... buf[len - 1] to the
 * end of the string stored in sa, allocating space if necessary, and
 * returns 1. If sa is unallocated, stralloc_catb is the same as
 * stralloc_copyb. If it runs out of memory, stralloc_catb leaves sa
 * alone and returns 0. */
int
stralloc_catb(stralloc* sa, const char* x, size_t n) {
  if(stralloc_readyplus(sa, n)) {
    byte_copy(sa->s + sa->len, n, x);
    sa->len += n;

    return 1;
  }

  return 0;
}
