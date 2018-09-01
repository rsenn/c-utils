#include "../strarray.h"

void
strarray_sort(strarray* arr) {
  char** strarray_start(arr);
  qsort(strarray_start(arr), strarray_size(arr), sizeof(char*), &str_diff);
}
