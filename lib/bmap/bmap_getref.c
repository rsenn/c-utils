#include "../bmap_internal.h"

bmap_node_t**
bmap_getref(bmap_base_t* m, const char* key) {
  unsigned hash = bmap_hash(key);
  bmap_node_t** next;

  if(m->nbuckets > 0) {
    next = &m->buckets[bmap_bucketidx(m, hash)];

    while(*next) {
      if((*next)->hash == hash && !str_diff((char*)(*next + 1), key)) {
        return next;
      }
      next = &(*next)->next;
    }
  }
  return NULL;
}
