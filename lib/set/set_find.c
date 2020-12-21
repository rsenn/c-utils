#include "../set.h"
#include <stdlib.h>
#include <string.h>

bucket_t* set_bucket(const set_t* set, const void* val, const size_t size);

uint32 set_index(const set_t* set, const void* val, const size_t size);

bucket_t**
set_find_bucket(bucket_t** ptr, const void* val, const size_t size, uint32 hash) {
  bucket_t* b;
  while((b = *ptr)) {
    if(size == b->size && b->hash == hash)
      if(memcmp(b->value, val, size) == 0)
        return ptr;
    ptr = &((*ptr)->next);
  }
  return 0;
}

bucket_t**
set_find(const set_t* set, const void* val, const size_t size) {
  bucket_t** ptr;
  uint32 hash = set_hash(set, val, size);
  set_iterator_t it;
  it.set = set;
  it.index = hash % set->len;
  it.current = &set->array[it.index];

  return set_find_bucket((bucket_t**)&it.current, val, size, hash);
  /*


    for(it.current = &set->array[it.index]; (it.current && it.current->value);
    it.current = it.current->next) { if(size == it.current->size &&
    it.current->hash == hash) if(memcmp(it.current->value, val, size) == 0)
          return it;
    }

    return it;*/
}