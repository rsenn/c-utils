#include "../set.h"
#include <string.h>

int
set_has(const set_t* set, const void* val, const size_t size) {
  uint32 hash, index;
  bucket_t* b;

  hash = set->hash_fp(val, size);
  index = hash & (set->len - 1);

  b = &(set->array[index]);

  while(b && b->value) {
    if(size == b->size) {
      if(b->hash == hash) {
        if(memcmp(b->value, val, size) == 0)
          return 1;
      }
    }
    b = b->next;
  }

  return 0;
}