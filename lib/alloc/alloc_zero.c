#include "../alloc.h"
#include "../byte.h"

void*
alloc_zero(size_t n) {
  void* ret = alloc(n);
  if(ret)
    byte_zero(ret, n);
  return ret;
}

