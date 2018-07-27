#include "../byte.h"

unsigned int
byte_case_equal(const void* s, unsigned int len, const void* t) {
  return byte_case_diff(s, len, t) == 0;
}
