#include "../map_internal.h"

void map_remove_(map_base_t* m, const char* key) {
  map_node_t* node;
  map_node_t** next = map_getref(m, key);
  if(next) {
    node = *next;
    *next = (*next)->next;
    free(node);
    m->nnodes--;
  }
}
