#include "../bmap_internal.h"

const char*
bmap_next_(bmap_base_t* m, bmap_iter_t* iter) {
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
