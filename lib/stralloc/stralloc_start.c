#include "../byte.h"
#include "../stralloc.h"

size_t
stralloc_start(const stralloc* sa, const stralloc* prefix) {
  size_t len;
  len = prefix->len;
  return (sa->len >= len) && byte_equal(prefix->s, len, sa->s);
}
