#include "../set.h"
#include <stdlib.h>
#include <assert.h>

bucket_t* set_bucket(const set_t* set, const void* val, const size_t size);

int
set_iterator_find(set_iterator_t* it, const void* x, size_t n) {
  set_t* s;
  bucket_t* b;

  if(!(b = (bucket_t*)it->current) || !(s = (set_t*)it->set))
    return 0;

  for(; b; b = (bucket_t*)b->next) {
  }
}
