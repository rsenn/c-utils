#include "../byte.h"

size_t
byte_case_start(const void* x1, size_t len, const void* x2) {
  const unsigned char* s;
  const unsigned char* t;
  int x;
  int y;
  s = (const unsigned char*)x1;
  t = (const unsigned char*)x2;
  for(;;) {
    y = *t++;
    if(y >= 'A' && y <= 'Z') { /* upper case */
      y += 'a' - 'A';
    }
    if(!y)
      return 1;
    if(!len)
      return 0;
    --len;
    x = *s++;
    if(x >= 'A' && x <= 'Z') { /* upper case */
      x += 'a' - 'A';
    }
    if(x != y)
      return 0;
  }
}
