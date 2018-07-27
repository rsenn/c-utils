#include "../str.h"

unsigned int
str_case_equal(const void* s, const void* t) {
  return str_case_diff(s, t) == 0;
}
