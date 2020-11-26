#include "../set.h"
#include <stdlib.h>
#include <assert.h>

char**
set_toarray(const set_t* set) {
  uint32 index, i = 0;
  bucket_t* b;
  char** array = malloc(sizeof(void*) * (set->entries + 1));
  assert(array);

  for(index = 0; index < set->len; ++index) {
    b = &(set->array[index]);

    while(b && b->value) {
      array[i++] = b->value;
      b = b->next;
    }
  }
  array[set->entries] = 0;
  return array;
}
