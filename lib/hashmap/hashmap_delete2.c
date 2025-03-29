#include "../hashmap.h"

void
hashmap_delete2(hashmap* map, const char* key, size_t keylen) {
  hashentry* ent;

  if((ent = hashmap_get_entry(map, key, keylen)))
    ent->key = HASHMAP_TOMBSTONE;
}
