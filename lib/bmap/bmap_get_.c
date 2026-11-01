#include "../bmap_internal.h"

void*
bmap_get_(bmap_base_t* m, const char* key) {
  bmap_node_t** next = bmap_getref(m, key);
  return next ? (*next)->value : NULL;
}
