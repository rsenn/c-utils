#include "../byte.h"
#include "../stralloc.h"

int
stralloc_prependb(stralloc* sa, const char* s, size_t n) {
  return stralloc_insertb(sa, s, 0, n);
}

