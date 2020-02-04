#include "../strarray.h"
#include <stdlib.h>

void
strarray_free(strarray* arr) {
  char* x;
  strarray_foreach(arr, x) {
    if(x) free(x);
  }
  array_reset(arr);
}
