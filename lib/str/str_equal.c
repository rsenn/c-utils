#include "../str.h"

#undef str_equal

size_t
str_equal(const void* s, const void* t) {
  return str_diff(s, t) == 0;
}
