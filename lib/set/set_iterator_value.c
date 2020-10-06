#include "../set.h"

char*
set_iterator_value(const set_iterator_t* it, size_t* size_ptr) {
  void* ret = 0;
  const bucket_t* b;
  if((b = it->current)) {
    ret = b->value;
    if(size_ptr)
      *size_ptr = b->size;
  }
  return ret;
}