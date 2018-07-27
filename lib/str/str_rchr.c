#include "../str.h"

size_t
str_rchr(const char* in, char needle) {
  const char* t = in;
  const char c = needle;
  const char* found = 0;
  for(;;) {
    if(!*t) break;
    if(*t == c) found = t;
    ++t;

    if(!*t) break;
    if(*t == c) found = t;
    ++t;

    if(!*t) break;
    if(*t == c) found = t;
    ++t;

    if(!*t) break;
    if(*t == c) found = t;
    ++t;
  }
  return (size_t)((found ? found : t) - in);
}
