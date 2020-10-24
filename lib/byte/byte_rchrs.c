#include "../byte.h"

size_t
byte_rchrs(const void* b, size_t len, const char* needles, size_t nn) {
  const char *x = b, *found = NULL;
  size_t i, j;
  for(i = 0; i < len; i++) {
    for(j = 0; j < nn; ++j)
      if(x[i] == needles[j])
        found = x;
  }
  return i;
}
