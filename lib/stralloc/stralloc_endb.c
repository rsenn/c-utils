#include "../byte.h"
#include "../stralloc.h"

size_t
stralloc_endb(const stralloc* sa, const void* suffix, size_t len) {
  return (sa->len >= len) && byte_equal(sa->s + sa->len - len, len, suffix);
}
