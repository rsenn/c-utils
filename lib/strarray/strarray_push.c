#include "../array.h"
#include "../str.h"
#include "../strarray.h"

int
strarray_push(strarray* arr, const char* s) {
  char* d = str_dup(s);
  array_catb((array*)arr, (void*)&d, sizeof(char*));
  return arr->allocated != -1;
}