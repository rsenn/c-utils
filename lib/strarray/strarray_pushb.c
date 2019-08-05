#include "../str.h"
#include "../strarray.h"

int
strarray_pushb(strarray* arr, const char* b, size_t n) {
  char* d;
  if((d = str_ndup(b, n))) {
    array_catb((array*)arr, (void*)&d, sizeof(char*));
  }
  return !!d;
}

