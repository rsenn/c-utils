#include "../byte.h"
#include "../str.h"

int
byte_ends(const void* x, size_t n, const char* s) {
  return byte_endb(x, n, s, str_len(s));
}
