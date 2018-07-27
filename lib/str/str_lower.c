#include "../str.h"

void
str_lower(void* str) {
  char* s;
  int x;
  s = str;
  while((x = *s)) {
    if(x >= 'A' && x <= 'Z') { /* upper case */
      *s = (char)(x + 'a' - 'A');
    }
    ++s;
  }
}
