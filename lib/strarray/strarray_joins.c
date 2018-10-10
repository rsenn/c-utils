#include "../strarray.h"
#include "../stralloc.h"
#include "../str.h"

int
strarray_joins(strarray* arr, stralloc* sa, const char* sep) {
  size_t i, n = strarray_size(arr);
  for(i = 0; i <n; ++i) {
    char** p = array_get(arr, sizeof(char*), i);
    stralloc_zero(sa);
    stralloc_cats(&sa, *p);
    if(i + 1 < n)
      stralloc_cats(&sa, sep);
  }
  return 1;
}
