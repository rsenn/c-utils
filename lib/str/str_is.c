#include "../str.h"

int
str_is(const char* s, int (*pred)(int)) {
  if(!*s)
    return 0;
  while(*s) {
    if(!pred(*s))
      return 0;
    ++s;
  }
  return 1;
}
