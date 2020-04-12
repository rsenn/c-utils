#include "../strarray.h"
#include "../str.h"
#include <stdlib.h>

typedef int(cmp_fn_t)(const void*, const void*);

int strarray_sort_compare(const char**a, const char**b) {
  return str_diff(*a, *b);
}

void
strarray_sort(strarray* arr, int (*compare)(const char**, const char**)) {
  if(!compare)
    compare = &strarray_sort_compare;
  qsort(strarray_begin(arr), strarray_size(arr), sizeof(char*), (cmp_fn_t*)compare);
}
