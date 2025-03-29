#include "../fmt.h"

size_t
fmt_hexb(void* out, const void* d, size_t len) {
  static const char hexchars[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
  const char* buf = (const char*)d;
  char* s = (char*)out;

  if(s) {
    for(size_t i = 0; i < len; ++i) {
      *s++ = hexchars[(*buf >> 4) & 15];
      *s++ = hexchars[*buf & 15];
      buf++;
    }
  }

  return len * 2;
}
