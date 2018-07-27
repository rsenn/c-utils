#include "../case.h"

int case_diffb(const void* S, size_t len, const void* T) {
  unsigned char x;
  unsigned char y;
  const char* s = (const char*)S;
  const char* t = (const char*)T;

  while(len > 0) {
    --len;
    x = *s++ - 'A';
    if(x <= 'Z' - 'A') x += 'a'; else x += 'A';
    y = *t++ - 'A';
    if(y <= 'Z' - 'A') y += 'a'; else y += 'A';
    if(x != y)
      return ((int)(unsigned int) x) - ((int)(unsigned int) y);
  }
  return 0;
}
