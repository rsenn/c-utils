#include "../set.h"
#include <stdlib.h>

bucket_t** set_find(const set_t*, const void*, const size_t);

int
set_delete(set_t* set, const void* val, const size_t size) {
  bucket_t **ptr, *b;

  if((b = *(ptr = set_find(set, val, size)))) {
    *ptr = (*ptr)->next;
    free(b);
    return 1;
  }
  return 0;
}
