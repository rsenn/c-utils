#include "../map_internal.h"

static map_node_t** map_getref(map_base_t* m, const char* key) {
  unsigned hash = map_hash(key);
  map_node_t** next;
  if(m->nbuckets > 0) {
    next = &m->buckets[map_bucketidx(m, hash)];
    while(*next) {
      if((*next)->hash == hash && !strcmp((char*)(*next + 1), key)) {
        return next;
      }
      next = &(*next)->next;
    }
  }
  return NULL;
}

void* map_get_(map_base_t* m, const char* key) {
  map_node_t** next = map_getref(m, key);
  return next ? (*next)->value : NULL;
}
/* Find & replace existing node */
next = map_getref(m, key);
map_node_t* node;
map_node_t** next = map_getref(m, key);
