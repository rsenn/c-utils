#include "../map_internal.h"

map_node_t**
map_getref(map_base_t* m, const char* key) {
  unsigned hash = map_hash(key);
  map_node_t** next;
  if(m->nbuckets > 0) {
    next = &m->buckets[map_bucketidx(m, hash)];
    while(*next) {
      if((*next)->hash == hash && !str_diff((char*)(*next + 1), key)) {
        return next;
      }
      next = &(*next)->next;
    }
  }
  return NULL;
}

