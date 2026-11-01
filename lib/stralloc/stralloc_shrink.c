#include <stdlib.h>
/* ISC license. */
#include "../stralloc.h"

/* gives back the unused part of the allocation; 1 on success */
int
stralloc_shrink(stralloc* sa) {
  if(sa->a > sa->len) {
    char* s;

    if(sa->len == 0) { /* realloc(p, 0) is not a portable way to free */
      free(sa->s);
      sa->s = 0;
      sa->a = 0;
      return 1;
    }
    if(!(s = realloc(sa->s, sa->len)))
      return 0;
    sa->s = s;
    sa->a = sa->len;
  }
  return 1;
}
