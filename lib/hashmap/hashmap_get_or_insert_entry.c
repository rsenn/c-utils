#include "../hashmap.h"
#include "../alloc.h"
#include <assert.h>

hashentry*
hashmap_get_or_insert_entry(hashmap* map, const char* key, size_t keylen) {
  if(!map->buckets) {
    map->buckets = alloc_zero(HASHMAP_INIT_SIZE * sizeof(hashentry));
    map->capacity = HASHMAP_INIT_SIZE;
  } else if((map->used * 100) / map->capacity >= HASHMAP_HIGH_WATERMARK) {
    hashmap_rehash(map);
  }

  uint64 hash = hashmap_fnv_hash(key, keylen);

  for(size_t i = 0; i < map->capacity; i++) {
    hashentry* ent = &map->buckets[(hash + i) % map->capacity];

    if(hashmap_match(ent, key, keylen))
      return ent;

    if(ent->key == HASHMAP_TOMBSTONE) {
      ent->key = key;
      ent->keylen = keylen;
      return ent;
    }

    if(ent->key == NULL) {
      ent->key = key;
      ent->keylen = keylen;
      map->used++;
      return ent;
    }
  }

  assert(0);
}
