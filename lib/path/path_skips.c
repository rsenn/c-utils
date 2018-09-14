#include "../path.h"

size_t
path_skips(const char* s) {
  const char* p = s;

  while(*p && !path_issep(*p)) ++p;

  while(*p && path_issep(*p)) ++p;

  return p - s;
}