#include "../alloc.h"
#include "../strarray.h"
#include <stdlib.h>

void
strarray_free(strarray* arr) {
  char** x;
  strarray_foreach(arr, x) {
    if(*x)
      alloc_free((void*)*x);
  }
  array_reset(&arr->a);
}
