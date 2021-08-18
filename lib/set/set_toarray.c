#include "../set.h"
#include "../str.h"
#include <stdlib.h>
#include <assert.h>

char**
set_toarray(const set_t* set) {
  uint32 index, i = 0;
  bucket_t* b;
  char** array = malloc(sizeof(void*) * (set->entries + 1));
  assert(array);

  for(b = set->list; b; b = b->list_next) {
    array[i++] = str_ndup(b->value, b->size);
  }
  array[i] = 0;
  return array;
}
