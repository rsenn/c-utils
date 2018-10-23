#include "../path_internal.h"
size_t
path_len(const char* s, size_t n) {
  const char *p = s, *e = s + n;
  while(p < e && !path_issep(*p)) ++p;
  return p - s;
}