#include "../array.h"
#include <stdlib.h>

void
array_fail(array* x) {
  if(x->p)
    free(x->p);
  x->p = 0;
  x->initialized = 0;
  x->allocated = -1;
}
