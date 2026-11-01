#include "../set.h"
#include <stdlib.h>
#include <string.h>

/* Removes all values, the set can be used again. */
void
set_clear(set_t* set) {
  size_t i;

  if(!set || !set->array)
    return;

  for(i = 0; i < set->len; ++i) {
    bucket_t* next = set->array[i].next;

    free(set->array[i].value);

    while(next) {
      bucket_t* n = next->next;

      free(next->value);
      free(next);
      next = n;
    }
  }

  memset(set->array, 0, set->len * sizeof(bucket_t));
  set->overflow = 0;
  set->entries = 0;
  set->list = 0;
  set->last = 0;
}
