#include "../strarray.h"
#include <stdlib.h>

typedef int(cmp_fn_t)(const void*, const void*);

void
strarray_sort(strarray* arr, int (*compare)(char*, char*)) {
  qsort(strarray_begin(arr), strarray_size(arr), sizeof(char*), (cmp_fn_t*)compare);
}

