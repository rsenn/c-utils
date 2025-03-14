#include "../fmt.h"

unsigned int
fmt_hexbs(void* out, const void* d, unsigned int len) {
  char hexchars[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
  char* s;
  const char* buf;
  unsigned int i;
  s = (char*)out;
  buf = (const char*)d;

  if(s) {
    for(i = 0; i < len; ++i) {
      if(i > 0)
        *s++ = ' ';
      *s++ = hexchars[(*buf >> 4) & 15];
      *s++ = hexchars[*buf & 15];
      buf++;
    }
  }
  return (len * 3) - 1;
}
