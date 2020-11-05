#include "../byte.h"
#include "../str.h"

int
byte_starts(const void* x, size_t n, const char* s) {
  return byte_startb(x, n, s, str_len(s));
}
