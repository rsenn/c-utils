#include "../byte.h"
#include "../stralloc.h"

unsigned int
stralloc_startb(const stralloc* sa, const void* prefix, unsigned int len) {
  return (sa->len >= len) && byte_equal(sa->s, len, prefix);
}
