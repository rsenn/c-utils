#include "../byte.h"

size_t
byte_case_equal(const void* s, size_t len, const void* t) {
  return byte_case_diff(s, len, t) == 0;
}
