#include "../stralloc.h"
#include "../strarray.h"
#include "../uint64.h"

int64
strarray_index_of_sa(strarray* arr, const stralloc* sa) {
  const char** p;
  uint64 i = 0;
  strarray_foreach(arr, p) {
    if(stralloc_equals(sa, *p))
      return i;
    ++i;
  }
  return -1;
}
