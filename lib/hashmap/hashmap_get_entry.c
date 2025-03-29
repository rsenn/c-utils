#include "../hashmap.h"
#include <assert.h>

hashentry*
hashmap_get_entry(hashmap* map, const char* key, size_t keylen) {
  if(!map->buckets)
    return NULL;

  uint64 hash = hashmap_fnv_hash(key, keylen);

  for(size_t i = 0; i < map->capacity; i++) {
    hashentry* ent = &map->buckets[(hash + i) % map->capacity];

    if(hashmap_match(ent, key, keylen))
      return ent;

    if(ent->key == NULL)
      return NULL;
  }

  assert(0);
}
