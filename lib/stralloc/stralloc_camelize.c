#include "../stralloc.h"
#include <ctype.h>

size_t
stralloc_camelize(stralloc* sa) {
  size_t i;
  char* x = sa->s;

  for(i = 0; i < sa->len; i++) {
    if(sa->s[i] == '-') {
      ++i;
      *x++ = toupper(sa->s[i]);
    } else {
      *x++ = sa->s[i];
    }
  }
  sa->len = x - sa->s;
  return sa->len;
}

