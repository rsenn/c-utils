#include "../set.h"
#include <stdlib.h>

/* Releases all memory of the set. It is empty afterwards, like a new SET(),
 * and can be used again. */
void
set_free(set_t* set) {
  uint32 i;

  if(!set || !set->array)
    return;

  for(i = 0; i < set->len; i++) {
    bucket_t *n = set->array[i].next, *next;

    free(set->array[i].value);

    for(; n; n = next) {
      next = n->next;
      free(n->value);
      free(n);
    }
  }

  free(set->array);
  set_init(set);
}
