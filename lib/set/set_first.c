#include "../set.h"

/* The value that was added first (or inserted last), NULL if the set is empty. */
char*
set_first(const set_t* set, size_t* len) {
  const bucket_t* b = set->list;

  if(b == NULL)
    return NULL;

  if(len)
    *len = b->size;

  return b->value;
}
