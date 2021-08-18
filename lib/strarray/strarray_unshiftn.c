#include "../strarray.h"

int
strarray_unshiftn(strarray* arr, const char* const* v, size_t n) {
  return array_splice(&arr->a, sizeof(char*), 0, 0, n, v);
}
