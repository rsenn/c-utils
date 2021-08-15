#include "../str.h"
#include "../stralloc.h"
#include "../strarray.h"
#include <stdlib.h>

int
strarray_transform(strarray* in, strarray* out, char* (*pred)(const char*)) {
  size_t i, n = strarray_size(in);
  char **d, **s = strarray_begin((array*)in);

  array_allocate((array*)out, sizeof(char*), n - 1);

  d = strarray_begin((array*)out);

  for(i = 0; i < n; ++i)
    *d++ = pred(*s++);

  return i;
}
