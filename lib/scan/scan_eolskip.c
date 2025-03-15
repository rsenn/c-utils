#include "../scan.h"

size_t
scan_eolskip(const char* s, size_t limit) {
  size_t n = 0;

  if(limit > 1 && s[0] == '\r' && s[1] == '\n')
    n += 2;
  else if(limit > 0 && s[0] == '\n')
    n += 1;

  return n;
}
