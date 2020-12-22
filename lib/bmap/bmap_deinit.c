#include "../bmap_internal.h"

void
bmap_deinit_(bmap_base_t* m) {
  bmap_node_t *next, *node;
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
