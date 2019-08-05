#include "../array.h"
#include "../byte.h"

int
array_equal(const array* const x, const array* const y) {
  if(x->initialized != y->initialized) return 0;
  return byte_equal(x->p, x->initialized, y->p);
}

