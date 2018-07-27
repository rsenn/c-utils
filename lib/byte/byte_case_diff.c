#include "../byte.h"
#include <stdint.h>

int
byte_case_diff(const void* x1, unsigned int len, const void* x2) {
  const unsigned char* s;
  const unsigned char* t;
  int x;
  int y;
  s = x1;
  t = x2;
  while(len > 0) {
    --len;
    x = *s++;
    if(x >= 'A' && x <= 'Z') { /* upper case */
      x += 'a' - 'A';
    }
    y = *t++;
    if(y >= 'A' && y <= 'Z') { /* upper case */
      y += 'a' - 'A';
    }
    if(x != y) return x - y;
  }
  return 0;
}
