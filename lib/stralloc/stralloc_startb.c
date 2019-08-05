#include "../byte.h"
#include "../stralloc.h"

size_t
stralloc_startb(const stralloc* sa, const void* prefix, size_t len) {
  return (sa->len >= len) && byte_equal(sa->s, len, prefix);
}

