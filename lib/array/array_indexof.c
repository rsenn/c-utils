#include "../array.h"
#include "../byte.h"

int64
array_indexof(array* a, uint64 membersize, void* data) {
  char* s = a->p;
  int64 i, n = array_length(a, membersize);
  for(i = 0; i < n; ++i) {
    if(byte_equal(s, membersize, data))
       return i;
    s += membersize;
  }
  return -1;
}
