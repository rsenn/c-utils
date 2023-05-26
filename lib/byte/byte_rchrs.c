#include "../byte.h"

size_t
byte_rchrs(const void* in, size_t len, const char needles[], size_t nn) {
  const char* s = (const char*)in + len;
  size_t i;

  while(--s >= (const char*)in)
    for(i = 0; i < nn; ++i)
      if(*s == needles[i])
        return s - (const char*)in;

  return len;
}
