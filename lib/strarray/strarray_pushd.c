#include "../str.h"
#include "../strarray.h"

int
strarray_pushd(strarray* arr, const char* s) {
  char* d;

  if((d = str_dup(s))) {
    array_catb((array*)arr, (void*)&d, sizeof(char*));
  }
  return !!d;
}
