#include "../strarray.h"
#include <stdlib.h>

void
strarray_sort(strarray* arr, int (*compare)(char*, char*)) {
  qsort(strarray_begin(arr), strarray_size(arr), sizeof(char*), (void*)compare);
}
