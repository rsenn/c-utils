#include <string.h>

char *
str_istr(char* s, char* find) {
  register char c, sc;
  register len;

  if((c = *find++) != 0) {
    len = strlen(find);
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

