#include "../byte.h"

void
byte_upper(void* s, size_t len) {
  unsigned char c;
  unsigned char* x;
  x = (unsigned char*)s;

  while(len) {
    --len;
    c = *x;

    if(c >= 'a' && c <= 'z') { /* lower case */
      *x = (unsigned char)(c - 32);
    }
    ++x;
  }
}
