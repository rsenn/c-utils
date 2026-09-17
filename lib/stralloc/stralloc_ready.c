#include "../byte.h"
#include "../stralloc.h"
#include <stdlib.h>

/* stralloc_ready makes sure that sa has enough space allocated to hold
 * len bytes: If sa is not allocated, stralloc_ready allocates at least
 * len bytes of space, and returns 1. If sa is already allocated, but
 * not enough to hold len bytes, stralloc_ready allocates at least len
 * bytes of space, copies the old string into the new space, frees the
 * old space, and returns 1. Note that this changes sa.s. */
int
stralloc_ready(stralloc* sa, size_t len) {
  size_t wanted = len + (len >> 3) + 30; /* heuristic from djb */
  char* tmp;

  if(sa->s && sa->a >= len)
    return 1;

  if(sa->a == 0 || sa->s == NULL) {
    if(!(tmp = malloc(wanted)))
      return 0;

    if(sa->s)
      /* copy however much valid content actually exists (sa->len),
       * not the newly requested target size (len) -- sa->a == 0 with
       * sa->s != NULL means sa doesn't own this buffer (e.g.
       * genmakefile's stralloc_weak()), so it's only guaranteed to be
       * readable for sa->len bytes; copying len (> sa->len whenever
       * this branch is reached to grow, e.g. via stralloc_nul()'s "+1")
       * read past the end of that borrowed buffer. */
      byte_copy(tmp, sa->len, sa->s);
    else
      byte_zero(tmp, wanted);

  } else {
    if(!(tmp = realloc(sa->s, wanted)))
      return 0;
  }

  sa->a = wanted;
  sa->s = tmp;

  return 1;
}
