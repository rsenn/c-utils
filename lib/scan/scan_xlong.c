#include "../scan.h"
#include "../str.h"
#include <ctype.h>

size_t
scan_xlong(const char* src, unsigned long* dest) {
  const char* tmp = src;
  unsigned long l = 0;
  unsigned char c;

  while((c = str_chr("0123456789abcdef", tolower(*tmp))) < 16) {
    l = (l << 4) + c;
    ++tmp;
  }
  *dest = l;
  return tmp - src;
}
