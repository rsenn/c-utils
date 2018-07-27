#include "../str.h"

unsigned int
str_copyb(void* p1, const void* p2, unsigned int max) {
  unsigned int len;
  char* s;
  const char* t;
  s = p1;
  t = p2;
  len = 0;
  while(max-- > 0) {
    if(!(*s = *t)) return len;
    ++s;
    ++t;
    ++len;

    if(!(*s = *t)) return len;
    ++s;
    ++t;
    ++len;

    if(!(*s = *t)) return len;
    ++s;
    ++t;
    ++len;

    if(!(*s = *t)) return len;
    ++s;
    ++t;
    ++len;
  }
  return len;
}
