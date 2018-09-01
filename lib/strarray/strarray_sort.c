#include <stdlib.h>
#include "../strarray.h"

void
strarray_sort(strarray* arr, int (*compare)(char*,char*)) {
  qsort(strarray_start(arr), strarray_size(arr), sizeof(char*), compare);
}
