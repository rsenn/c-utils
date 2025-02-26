#include "../strarray.h"
#include <stdarg.h>

int
strarray_unshiftm(strarray* arr, ...) {
  int r = 0;
  int i = 0;
  va_list args;
  const char* s;
  va_start(args, arr);

  while((s = va_arg(args, const char*))) {
    if(strarray_splice(arr, i, 0, 1, &s) != -1) {
      i++;
    } else {
      r = -1;
      break;
    }
  }
  va_end(args);
  return r;
}
