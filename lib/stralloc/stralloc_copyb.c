#include "../byte.h"
#include "../stralloc.h"

/* stralloc_copyb copies the string buf[0], buf[1], ..., buf[len - 1] into
 * sa, allocating space if necessary, and returns 1. If it runs out of
 * memory, stralloc_copyb leaves sa alone and returns 0. */
int
stralloc_copyb(stralloc* sa, const char* x, size_t n) {
  if(stralloc_ready(sa, n)) {
    sa->len = n;
    byte_copy(sa->s, n, x);
    return 1;
  }

  return 0;
}
