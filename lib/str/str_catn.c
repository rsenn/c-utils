#include "../str.h"

size_t
str_catn(void* p1, const void* p2, size_t max) {
  size_t len;
  char* s;
  const char* t;
  s = p1;
  t = p2;
  len = 0;

  while(*s && len < max) {
    ++s;
    ++len;
  }

  while(len < max) {
    if(!(*s = *t))
      return len;
    ++s;
    ++t;
    ++len;
  }
  *s = '\0';
  return len;
}
