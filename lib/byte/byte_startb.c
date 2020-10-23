#include "../byte.h"

int
byte_startb(const void* b, size_t blen, const char* x, size_t xlen) {
  if(blen >= xlen)
    return byte_equal(b, xlen, x);
  return 0;
}
