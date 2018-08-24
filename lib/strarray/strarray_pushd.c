#include "../strarray.h"
#include "../str.h"

int
strarray_pushd(strarray* arr, const char* s) {
  char* d;

  if((d = str_dup(s))) {
    array_catb(&arr->a, (void*)&d, sizeof(char*));
 }
  return  !!d;
}
