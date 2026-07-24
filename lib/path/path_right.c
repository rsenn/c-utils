#include "../path_internal.h"
#include "../utf8.h"

size_t
path_right(const char* s, size_t n) {
  const char* p;

  if(n == 0)
    return 0;

  p = s + n - 1;

  while(p >= s && path_issep(*p))
    --p;

  while(p >= s && !path_issep(*p))
    --p;

  if(p < s)
    return 0;

  return p - s;
}
