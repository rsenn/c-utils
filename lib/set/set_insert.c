#include "../set_internal.h"

/* Like set_add, but a new value goes before the others. */
int
set_insert(set_t* set, const void* val, const size_t size) {
  return set_put(set, val, size, 1);
}
