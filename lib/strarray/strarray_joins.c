#include "../strarray.h"
#include "../stralloc.h"
#include "../str.h"

int
strarray_joins(strarray* arr, stralloc* sa, const char* sep) {
  size_t i, n = strarray_size(arr);
  char** p = array_start(arr);
    stralloc_zero(sa);

  for(i = 0; i <n; ++i) {
    stralloc_cats(&sa, p[i]);
    if(i + 1 < n)
      stralloc_cats(&sa, sep);
  }
  return 1;
}
