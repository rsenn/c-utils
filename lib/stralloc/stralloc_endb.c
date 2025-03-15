#include "../byte.h"
#include "../stralloc.h"

size_t
stralloc_endb(const stralloc* sa, const char* x, size_t n) {
  return sa->len >= n && byte_equal(sa->s + sa->len - n, n, x);
}
