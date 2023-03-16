#include "../array.h"
#include "../str.h"
#include "../strarray.h"

int
strarray_copy(strarray* dst, const strarray* src) {
  char **s, **d;

  if(!(d = array_allocate(&dst->a, sizeof(char*), strarray_size(src))))
    return 0;

  *d = NULL;
  d = strarray_begin(dst);

  strarray_foreach(src, s) {
    *d = *s ? str_dup(*s) : 0;
    ++d;
  }

  return 1;
}
