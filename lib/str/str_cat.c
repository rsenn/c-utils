#include "../str.h"

size_t
str_cat(void* p1, const void* p2) {
  char* s = p1;
  const char* t = p2;
  size_t len = str_len(s);
  s += len;

  for(;;) {
    if(!(*s = *t))
      return len;
    ++s;
    ++t;
    ++len;

    if(!(*s = *t))
      return len;
    ++s;
    ++t;
    ++len;

    if(!(*s = *t))
      return len;
    ++s;
    ++t;
    ++len;

    if(!(*s = *t))
      return len;
    ++s;
    ++t;
    ++len;
  }
}
