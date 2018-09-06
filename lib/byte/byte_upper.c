#include "../byte.h"

void
byte_upper(void* s, unsigned int len) {
  unsigned char c;
  unsigned char* x;
  x = s;
  while(len) {
    --len;
    c = *x;
    if(c >= 'a' && c <= 'z') { /* lower case */
      *x = (unsigned char)(c - 32);
    }
    ++x;
  }
}
