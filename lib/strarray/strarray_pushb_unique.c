#include "../strarray.h"

int
strarray_pushb_unique(strarray* arr, const char* b, size_t n) {
  if(strarray_index_of_b(arr, b, n) == -1)
    return strarray_pushb(arr, b, n);

  return 0;
}
