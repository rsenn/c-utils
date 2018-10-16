#include "../str.h"
#include "../stralloc.h"
#include "../strarray.h"
#include <stdlib.h>

int
strarray_transform(strarray* in, strarray* out, char* (*pred)(const char*)) {
  size_t i, n = strarray_size(in);
  char **d, **s = array_start(&in->a);

  array_allocate(&out->a, sizeof(char*), n - 1);

  d = array_start(&out->a);

  for(i = 0; i < n; ++i)
    *d++ = pred(*s++);

  return i;
}