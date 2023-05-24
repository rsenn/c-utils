#include "../byte.h"

size_t
byte_rchrs(const void* in, size_t len, const char needles[], size_t nn) {
  const char *s = in, *e = in + len, *found = NULL;
  size_t i;
  for(; s < e;) {
    for(i = 0; i < nn; ++i) {
      if(*s == needles[i])
        found = s;
    }
    ++s;
  }
  return (size_t)((found ? found : s) - (const char*)in);
}
