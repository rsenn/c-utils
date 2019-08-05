#include "../str.h"
#include "../strarray.h"
#include "../uint64.h"

int64
strarray_index_of(strarray* arr, const char* s) {
  size_t i, n = strarray_size(arr);
  for(i = 0; i < n; ++i) {
    const char* x = strarray_at(arr, i);
    if(x && str_equal(s, x)) return i;
  }
  return -1;
}

