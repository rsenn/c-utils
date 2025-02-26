#include "../bmap_internal.h"

void
bmap_remove_(bmap_base_t* m, const char* key) {
  bmap_node_t* node;
  bmap_node_t** next = bmap_getref(m, key);

  if(next) {
    node = *next;
    *next = (*next)->next;
    free(node);
    m->nnodes--;
  }
}
