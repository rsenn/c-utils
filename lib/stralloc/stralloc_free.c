#include "../stralloc.h"
#include <stdlib.h>

/* releases the memory and leaves an empty stralloc that can be used again
 * (len and a used to stay behind, so a reuse wrote through a NULL pointer) */
void
stralloc_free(stralloc* sa) {
  if(sa->s && sa->a)
    free(sa->s);
  sa->s = 0;
  sa->len = 0;
  sa->a = 0;
}
