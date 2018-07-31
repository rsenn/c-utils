#include <stdlib.h>
#include "../strarray.h"
#include "../uint64.h"

int64
strarray_index_of(strarray* arr, const char* s) {
  size_t i, n =  strarray_size(arr);
  for(i = 0; i < n; ++i) {
    if(str_equal(s, strarray_at(arr, i)))
      return i;
  }
  return -1;
}

