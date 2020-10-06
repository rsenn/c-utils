#include "../set.h"
#include <stdlib.h>
#include <assert.h>

int
set_iterator_erase(set_iterator_t* it) {
  set_t* s;
  bucket_t* b;
  uint32 i;

  if(!(b = (bucket_t*)it->current) || !(s = (set_t*)it->set))
    return 0;

  i = it->index;
}
