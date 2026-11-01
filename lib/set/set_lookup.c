#include "../set_internal.h"
#include <string.h>

bucket_t*
set_lookup(const set_t* set, const void* val, size_t size, uint32 hash) {
  bucket_t* b;

  if(set->array == NULL)
    return NULL;

  for(b = &set->array[hash & (set->len - 1)]; b && b->value; b = b->next)
    if(size == b->size && b->hash == hash && memcmp(b->value, val, size) == 0)
      return b;

  return NULL;
}
