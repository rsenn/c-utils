#include "../fmt.h"

size_t
fmt_hexb(void* out, const void* d, size_t len) {
   const char* buf = (const char*)d;
  char* s = (char*)out;

  if(s) {
    for(size_t i = 0; i < len; ++i) {
      *s++ = fmt_hexchars[(*buf >> 4) & 15];
      *s++ = fmt_hexchars[*buf & 15];
      buf++;
    }
  }

  return len * 2;
}
