#include "../fmt.h"

size_t
fmt_octal(char* dest, uint64 i) {
  size_t len;
  uint64 tmp;
  /* first count the number of bytes needed */
  for(len = 1, tmp = i; tmp > 7; ++len) tmp >>= 3;
  if(dest)
    for(tmp = i, dest += len;;) {
      *--dest = (char)((tmp & 7) + '0');
      if(!(tmp >>= 3)) {
        break;
      };
    }
  return len;
}

