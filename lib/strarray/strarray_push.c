#include "../array.h"
#include "../str.h"
#include "../strarray.h"

int
strarray_push(strarray* arr, char* s) {
  char* d = str_dup(s);
  array_catb(&arr->a, (void*)&d, sizeof(char*));
  return arr->a.allocated != -1;
}
