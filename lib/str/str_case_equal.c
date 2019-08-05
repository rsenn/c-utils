#include "../str.h"

size_t
str_case_equal(const void* s, const void* t) {
  return str_case_diff(s, t) == 0;
}

