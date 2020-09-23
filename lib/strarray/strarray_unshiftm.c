#include "../strarray.h"
#include <stdarg.h>

int
strarray_unshiftm(strarray* arr, ...) {
  int r = 0;
  va_list args;
  const char* s;
  va_start(args, arr);
  while((s = va_arg(args, const char*)))
    if(strarray_unshift(arr, s) == -1) {
      r = -1;
      break;
    }
  va_end(args);
  return r;
}
