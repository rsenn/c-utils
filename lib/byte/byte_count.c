#include "byte.h"
#include "stdint.h"

unsigned int
byte_count(const void* s, register unsigned int n, char c) {
  register const unsigned char* t;
  register unsigned int count;
  register unsigned char ch;
  t = s;
  count = 0;
  ch = (unsigned char)c;
  for(;;) {
    if(!n)
      break;
    if(*t == ch) {
      ++count;
    }
    ++t;
    --n;

    if(!n)
      break;
    if(*t == ch) {
      ++count;
    }
    ++t;
    --n;

    if(!n)
      break;
    if(*t == ch) {
      ++count;
    }
    ++t;
    --n;

    if(!n)
      break;
    if(*t == ch) {
      ++count;
    }
    ++t;
    --n;
  }
  return count;
}
