#include "../str.h"

int
str_endb(const char* a, const char* x, size_t n) {
  size_t alen = str_len(a);
  a += alen - n;
  return alen >= n && byte_equal(a, n, x);
}
