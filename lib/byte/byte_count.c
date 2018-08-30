#include "../byte.h"
#include <stdint.h>

unsigned int
byte_count(const void* s, unsigned int n, char c) {
  const unsigned char* t;
  unsigned int count;
  unsigned char ch;
  t = s;
  count = 0;
  ch = (unsigned char)c;
  for(;;) {
    if(!n) break;
    if(*t == ch) {
      ++count;
    }
    ++t;
    --n;

    if(!n) break;
    if(*t == ch) {
      ++count;
    }
    ++t;
    --n;

    if(!n) break;
    if(*t == ch) {
      ++count;
    }
    ++t;
    --n;

    if(!n) break;
    if(*t == ch) {
      ++count;
    }
    ++t;
    --n;
  }
  return count;
}
