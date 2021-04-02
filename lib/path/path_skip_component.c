#include "../path_internal.h"

size_t
path_skip_component(const char* s, size_t n) {
  const char *p = s, *e = s + n;
  for(p = s, e = s + n; p != e; p++)
    if(path_issep(*p))
      break;
  return p - s;
}
