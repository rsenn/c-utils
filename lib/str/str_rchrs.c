#include "../str.h"

size_t
str_rchrs(const char* in, char needles[], size_t nn) {
  const char* found = NULL;
  size_t i;
  for(;;) {
    if(!*in) break;
    for(i = 0; i < nn; ++i) {
      if(*in == needles[i])
        found = in;
    }
    ++in;
  }
  return (size_t)((found ? found : in) - in);
}