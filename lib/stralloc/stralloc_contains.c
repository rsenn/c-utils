#include "../stralloc.h"

int
stralloc_contains(const stralloc* sa, const char* what) {
  if(sa->s == 0)
    return 0;
  return stralloc_finds(sa, what) != sa->len;
}
