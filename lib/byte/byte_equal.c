#include "byte.h"

unsigned int
byte_equal(const void* s, unsigned int n, const void* t) {
  return (byte_diff(s, n, t) == 0);
}
