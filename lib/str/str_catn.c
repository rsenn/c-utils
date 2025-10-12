#include "../str.h"

size_t
str_catn(void* p1, const void* p2, size_t max) {
  char* s = p1;
  const char* t = p2;
  size_t len = 0;

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
