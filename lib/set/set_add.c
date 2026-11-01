#include "../set_internal.h"

/* Adds a copy of the size bytes at val after the others. Returns SET_ADDED,
 * SET_EXISTS if the set holds the value already (it keeps its place) or
 * SET_ERROR if the memory ran out (the set is unchanged). */
int
set_add(set_t* set, const void* val, const size_t size) {
  return set_put(set, val, size, 0);
}
