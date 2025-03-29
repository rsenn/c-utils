#include "../hashmap.h"

void
hashmap_put2(hashmap* map, const char* key, size_t keylen, void* val) {
  hashentry* ent = hashmap_get_or_insert_entry(map, key, keylen);

  ent->val = val;
}
