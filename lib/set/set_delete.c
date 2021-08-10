#include "../set.h"
#include <stdlib.h>

bucket_t** set_find(const set_t*, const void*, const size_t);

int
set_delete(set_t* set, const void* val, const size_t size) {
  bucket_t **ptr, *b;

  if((b = *(ptr = set_find(set, val, size)))) {
    bucket_t *prev = b->list_prev, *next = b->list_next;

    *ptr = (*ptr)->next;
    free(b);

    if(next)
      next->list_prev = prev;
    else
      set->last = prev;

    if(prev)
      prev->list_next = next;
    else
      set->list = next;

    return 1;
  }
  return 0;
}
