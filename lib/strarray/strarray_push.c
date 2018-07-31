#include "../strarray.h"
#include "../array.h"

int
strarray_push(strarray* arr, char* s) {
  array_catb(&arr->a, (void*)&s, sizeof(char*));
   return arr->a.allocated != -1;
}
