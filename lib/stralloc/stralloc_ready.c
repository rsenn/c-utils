#include <stdlib.h>
#include "../stralloc.h"
#include "../str.h"

/* stralloc_ready makes sure that sa has enough space allocated to hold
 * len bytes: If sa is not allocated, stralloc_ready allocates at least
 * len bytes of space, and returns 1. If sa is already allocated, but
 * not enough to hold len bytes, stralloc_ready allocates at least len
 * bytes of space, copies the old string into the new space, frees the
 * old space, and returns 1. Note that this changes sa.s. */
int stralloc_ready(stralloc* sa, size_t len) {
  size_t wanted = len + (len >> 3) + 30; /* heuristic from djb */
  char* tmp;
  if(sa->s && sa->a >= len)
    return 1;
  if(sa->a == 0 || sa->s == NULL) {
    wanted = sa->len;
    if(!(tmp = str_ndup(sa->s, wanted)))
      return 0;
  } else {
    if(!(tmp = realloc(sa->s, wanted)))
      return 0;
  }
    sa->a = wanted;
  sa->s = tmp;
  return 1;
}
