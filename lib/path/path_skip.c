#include "../path_internal.h"

size_t
path_skip(const char* s, size_t n) {
  const char *p = s, *e = s + n;

  while(p < e && !path_issep(*p)) ++p;

  while(p + 1 < e && path_issep(*p)) ++p;

  return p - s;
}