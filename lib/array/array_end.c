#include "../array.h"

void*
array_end(const array* const x) {
  return x->p + x->initialized;
}
