#include "../str.h"
#include "../stralloc.h"
#include "../strarray.h"
#include <stdlib.h>

int
strarray_transform(strarray* arr, char* (*pred)(const char*)) {
  size_t i, n = strarray_size(arr);
  char** p = array_start(arr);
  for(i = 0; i < n; ++i) {
    char* s = *p;
    char* o = pred(s);

    if(str_diff(s, o)) {
      *p = o;
      free(s);
    }
    ++p;
  }
  return 1;
}
