#include "../strarray.h"

int
strarray_each(strarray* arr, void (*pred)(char**)) {
  size_t i, n = strarray_size(arr);
  char** p = array_start((array*)arr);

  for(i = 0; i < n; ++i)
    pred(p++);

  return i;
}