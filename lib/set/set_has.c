#include "../set_internal.h"

/* 1 if the set holds the size bytes at val, otherwise 0. */
int
set_has(const set_t* set, const void* val, const size_t size) {
  if(set->array == NULL)
    return 0;

  return set_lookup(set, val, size, set_hash(set, val, size)) != NULL;
}
