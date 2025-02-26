#include "../set.h"
#include <string.h>

uint32
set_index(const set_t* set, const void* val, const size_t size) {
  uint32 hash;
  hash = set_hash(set, val, size);
  return hash & (set->len - 1);
}

int
set_has(const set_t* set, const void* val, const size_t size) {
  bucket_t* b;
  uint32 hash, i;
  hash = set_hash(set, val, size);

  i = hash % set->len;
  b = &set->array[i];

  for(b = &set->array[i]; (b && b->value); b = b->next) {
    if(size == b->size && b->hash == hash)

      if(memcmp(b->value, val, size) == 0) {
        return 1;
      }
  }

  return 0;
}
