#include "../str.h"

unsigned int
str_case_start(const void* p1, const void* p2) {
  int x;
  int y;
  const char* s;
  const char* t;
  s = p1;
  t = p2;
  for(;;) {
    x = *s++;
    if(x >= 'A' && x <= 'Z') { /* upper case */
      x += 'a' - 'A';
    }
    y = *t++;
    if(y >= 'A' && y <= 'Z') { /* upper case */
      y += 'a' - 'A';
    }
    if(!x) return 0;
    if(!y) return 1;
    if(x != y) return 0;
  }
}
