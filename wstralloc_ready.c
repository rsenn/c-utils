#include "wstralloc.h"
#include <stdlib.h>

/* wstralloc_ready makes sure that sa has enough space allocated to hold
 * len bytes: If sa is not allocated, wstralloc_ready allocates at least
 * len bytes of space, and returns 1. If sa is already allocated, but
 * not enough to hold len bytes, wstralloc_ready allocates at least len
 * bytes of space, copies the old wstring into the new space, frees the
 * old space, and returns 1. Note that this changes sa.s. */
int wstralloc_ready(wstralloc* sa, size_t len) {
  register size_t wanted = len + (len >> 3) + 30; /* heuristic from djb */
  if(!sa->s || sa->a < len) {
    register wchar_t* tmp;
    if(!(tmp = realloc(sa->s, wanted)))
      return 0;
    sa->a = wanted;
    sa->s = tmp;
  }
  return 1;
}
