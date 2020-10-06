#include "../set.h"
#include <string.h>

bucket_t* set_bucket(const set_t* set, const void* val, const size_t size);

uint32 set_index(const set_t* set, const void* val, const size_t size);

int
set_find(const set_t* set, const void* val, const size_t size) {
  bucket_t* b;
  uint32 hash, i;
  hash = set_hash(set, val, size);
  i =  hash % set->len;
  b = &set->array[i];

  for(b = &set->array[i]; (b && b->value); b = b->next) {
    if(size == b->size && b->hash == hash)
      if(memcmp(b->value, val, size) == 0) {
        return b;
      }
  }

  return 0;
}