#include "../stralloc.h"

#undef stralloc_0

size_t
stralloc_0(stralloc* sa) {
  return stralloc_append(sa, "");
}

