#include "../str.h"
#include <string.h>

char*
str_istr(char* s, char* find) {
  char c, sc;
  size_t len;

  if((c = *find++) != 0) {
    len = str_len(find);
    do {
      do {
        if((sc = *s++) == 0) return NULL;
      } while(sc != c);
    } while(str_case_diffn(s, find, len) != 0);
    s--;
  }
  return s;
}
