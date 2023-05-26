#include "../set.h"
#include <stdlib.h>

void
set_free_array(set_t* set) {
  bucket_t* next = NULL;
  uint32 i, n = set->len;

  for(uint32 i = 0; i < n; i++) {
    next = set->array[i].next;
    free(set->array[i].value);

    while(next) {
      set->array[i].next = next->next;
      free(next->value);
      free(next);
      next = set->array[i].next;
    }
  }

  free(set->array);
}

void
set_free(set_t* set) {
  if(!set)
    return;

  set_free_array(set);
}
