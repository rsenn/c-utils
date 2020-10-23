#include "../byte.h"

int
byte_endb(const void* b, size_t blen, const char* x, size_t xlen) {
  if(blen >= xlen)
    return byte_equal((const char*)b + blen - xlen, xlen, x);
  return 0;
}
