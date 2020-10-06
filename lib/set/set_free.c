#include "../set.h"
#include <stdlib.h>

void
set_free_array(set_t* set) {
  bucket_t* next = NULL;
  size_t i;

  for(i = 0; i < set->len; ++i) {
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
  free(set);
}