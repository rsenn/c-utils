#include "../set.h"

/* The value the iterator is at, 0 at the end. The size, if wanted, is stored
 * in *size_ptr. The value ends in a NUL byte that the size does not count. */
char*
set_iterator_value(const set_iterator_t* it, size_t* size_ptr) {
  const bucket_t* b = it->current;

  if(b == NULL)
    return NULL;

  if(size_ptr)
    *size_ptr = b->size;

  return b->value;
}
