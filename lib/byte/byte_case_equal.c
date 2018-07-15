#include "../byte.h"

unsigned int
byte_case_equal(register const void* s, register unsigned int len, register const void* t) {
  return byte_case_diff(s, len, t) == 0;
}
