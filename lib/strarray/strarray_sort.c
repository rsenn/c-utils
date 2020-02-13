#include "../strarray.h"
#include "../str.h"
#include <stdlib.h>

typedef int(cmp_fn_t)(const void*, const void*);

void
strarray_sort(strarray* arr, int (*compare)(const char**, const char**)) {
  if(!compare)
    compare = &str_diff;
  qsort(strarray_begin(arr), strarray_size(arr), sizeof(char*), (cmp_fn_t*)compare);
}
