#include "../str.h"
#include "../strarray.h"

int
strarray_unshiftb(strarray* arr, const char* b, size_t n) {
  char* d;

  if((d = str_ndup(b, n)))
    return array_splice(&arr->a, sizeof(char*), 0, 0, 1, &d) != -1;
  return 0;
}
