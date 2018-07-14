#include "byte.h"
#include "stdint.h"

void
byte_lower(void* s, register unsigned int len) {
  register unsigned char c;
  register unsigned char* x;
  x = s;
  while(len) {
    --len;
    c = *x;
    if(c >= 'A' && c <= 'Z') { /* upper case */
      *x = (unsigned char)(c + 32);
    }
    ++x;
  }
}
