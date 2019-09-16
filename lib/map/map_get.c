#include "../map_internal.h"

void*
map_get_(map_base_t* m, const char* key) {
  map_node_t** next = map_getref(m, key);
  return next ? (*next)->value : NULL;
}
