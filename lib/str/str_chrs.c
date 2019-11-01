#include "../str.h"

size_t
str_chrs(const char* in, char needles[], size_t nn) {
  const char* t = in;
  size_t i;
  for(;;) {
    if(!*t)
      break;
    for(i = 0; i < nn; i++)
      if(*t == needles[i])
        return (size_t)(t - in);
    ++t;
  }
  return (size_t)(t - in);
}
