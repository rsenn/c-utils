#include "../strarray.h"

char*
strarray_pop(strarray* arr) {
  char* ret = NULL;
  size_t n;
  if((n = strarray_size(arr)) > 0) {
    ret = strarray_AT(arr, n - 1);
    array_truncate((array*)arr, sizeof(char*), n - 1);
  }
  return ret;
}
