#include "../stralloc.h"

int
stralloc_insert(stralloc* sa, const stralloc* other, size_t pos) {
  return stralloc_insertb(sa, other->s, pos, other->len);
}

