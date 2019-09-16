#include "../str.h"
#include <ctype.h>

size_t
str_camelize(char* s) {
  char *p, *x;
  for(x = p = s; *p; p++) {
    if(*p == '-') {
      *x++ = toupper(*++p);
    } else {
      *x++ = *p;
    }
  }
  *x = '\0';
  return x - s;
}
