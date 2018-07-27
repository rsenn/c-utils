#include "../map_internal.h"

map_iter_t map_iter_(void) {
  map_iter_t iter;
  iter.bucketidx = -1;
  iter.node = NULL;
  return iter;
}

const char* map_next_(map_base_t* m, map_iter_t* iter) {
  if(iter->node) {
    iter->node = iter->node->next;
    if(iter->node == NULL)
      goto nextBucket;
  } else {
  nextBucket:
    do {
      if(++iter->bucketidx >= m->nbuckets) {
        return NULL;
      }
      iter->node = m->buckets[iter->bucketidx];
    } while(iter->node == NULL);
  }
  return (char*)(iter->node + 1);
}
