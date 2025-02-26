#include "../str.h"
#include "../stralloc.h"
#include "../strarray.h"

int
strarray_joins(const strarray* arr, stralloc* sa, const char* sep) {
  size_t i, n = strarray_size(arr);
  stralloc_zero(sa);

  for(i = 0; i < n; ++i) {
    stralloc_cats(sa, strarray_AT(arr, i));

    if(i + 1 < n)
      stralloc_cats(sa, sep);
  }
  return 1;
}
