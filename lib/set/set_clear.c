#include "../set.h"
#include <stdlib.h>
#include <string.h>

void
set_clear(set_t* set) {
  bucket_t* next = NULL;
  size_t i;

  if(!set)
    return;

  if(set->overflow) {
    // free all 'overflow' bucket entries

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
  }

  memset(set->array, 0, set->len * sizeof(bucket_t));

  set->overflow = 0;
  set->entries = 0;
  set->list = 0;
  set->last = 0;
}
