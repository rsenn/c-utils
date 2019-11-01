#include "../array.h"
#include <stdlib.h>

void
array_reset(array* x) {
  if(x->p)
    free(x->p);
  x->p = 0;
  x->allocated = x->initialized = 0;
}
