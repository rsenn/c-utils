#include "../array.h"
#include "../byte.h"

void*
array_find(array* a, uint64 membersize, void* data) {
  uint64 i, n = array_length(a, membersize);
    char* s = array_start(a);

    for(i = 0; i < n; ++i) {
    if(byte_equal(s, (size_t)membersize, data)) return s;
    s += (size_t)membersize;
  }
  return 0;
}
