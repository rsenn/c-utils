#include "../set.h"

/* The value at the end of the order, NULL if the set is empty. */
char*
set_last(const set_t* set, size_t* len) {
  const bucket_t* b = set->last;

  if(b == NULL)
    return NULL;

  if(len)
    *len = b->size;

  return b->value;
}
