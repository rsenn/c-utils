#include <string.h>
#include "../str.h"

char *
str_istr(char* s, char* find) {
  register char c, sc;
  register size_t len;

  if((c = *find++) != 0) {
    len = str_len(find);
    do {
      do {
        if((sc = *s++) == 0)
          return NULL;
      } while(sc != c);
    } while(strncasecmp(s, find, len) != 0);
    s--;
  }
  return s;
}

