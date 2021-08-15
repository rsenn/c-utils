#include "../path_internal.h"

size_t
path_skips(const char* s) {
  register const char* p = s;
  while(*p && !path_issep(*p))
    ++p;
  while(*p && path_issep(*p))
    ++p;
  return p - s;
}
