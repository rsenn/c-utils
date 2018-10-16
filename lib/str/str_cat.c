#include "../str.h"

size_t
str_cat(void* p1, const void* p2) {
  size_t len;
  char* s;
  const char* t;
  s = p1;
  t = p2;
  len = str_len(s);
  s += len;
  for(;;) {
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
}
