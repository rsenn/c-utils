#include "str.h"

unsigned int str_case_equal(register const void *s, register const void *t)
{
  return str_case_diff(s, t) == 0;
}
