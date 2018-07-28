#include "../str.h"

size_t
str_rchr(const char* in, char needle) {
  const char* found = NULL;
  for(;;) {
    if(!*in) break;
    if(*in == needle) found = in;
    ++in;
  }
  return (size_t)((found ? found : in) - in);
}