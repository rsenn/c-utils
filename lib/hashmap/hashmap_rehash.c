#include "../hashmap.h"
#include "../alloc.h"
#include <assert.h>

/* Make room for new entires in a given hashmap by removing
   tombstones and possibly extending the bucket size. */
void
hashmap_rehash(hashmap* map) {
  /* Compute the size of the new hashmap. */
  size_t nkeys = 0;

  for(size_t i = 0; i < map->capacity; i++)
    if(map->buckets[i].key && map->buckets[i].key != HASHMAP_TOMBSTONE)
      nkeys++;

  size_t cap = map->capacity;

  while((nkeys * 100) / cap >= HASHMAP_LOW_WATERMARK)
    cap = cap * 2;

  assert(cap > 0);

  /* Create a new hashmap and copy all key-values.*/
  hashmap map2 = {alloc_zero(cap * sizeof(hashentry)), cap};

  for(size_t i = 0; i < map->capacity; i++) {
    hashentry* ent = &map->buckets[i];

    if(ent->key && ent->key != HASHMAP_TOMBSTONE)
      hashmap_put2(&map2, ent->key, ent->keylen, ent->val);
  }

  assert(map2.used == nkeys);
  *map = map2;
}
