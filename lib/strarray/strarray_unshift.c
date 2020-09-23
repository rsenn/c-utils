#include "../strarray.h"

int
strarray_unshift(strarray* arr, const char* s) {
  return array_splice(arr, sizeof(char*), 0, 0, 1, &s);
}
