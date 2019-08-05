#include "../strarray.h"

char*
strarray_pop(strarray* arr, char* s) {
  char* ret = NULL;
  size_t n;
  if((n = strarray_size(arr)) > 0) {
    ret = strarray_at(arr, n - 1);
    array_truncate((array*)arr, sizeof(char*), n - 1);
  }
  return ret;
}

