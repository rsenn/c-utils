#include "../map_internal.h"

void map_deinit_(map_base_t* m) {
  map_node_t *next, *node;
  int i;
  i = m->nbuckets;
  while(i--) {
    node = m->buckets[i];
    while(node) {
      next = node->next;
      free(node);
      node = next;
    }
  }
  free(m->buckets);
}
