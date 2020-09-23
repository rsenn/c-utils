#include "../str.h"
#include "../strarray.h"

int
strarray_unshiftb(strarray* arr, const char* b, size_t n) {
  char* d;
  if((d = str_ndup(b, n))) 
    array_splice((array*)arr, 0, 0, 1, (void*)&d);
  return !!d;
}
