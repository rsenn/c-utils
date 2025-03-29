#include "../scan.h"
#include "../str.h"
#include <ctype.h>

size_t
scan_xlongn(const char* src, size_t n, unsigned long* dest) {
  const char* tmp = src;
  unsigned long l = 0;
  unsigned char c;

  while(n-- > 0 && (l >> (sizeof(l) * 8 - 4)) == 0 && (c = str_chr("0123456789abcdef", tolower(*tmp))) < 16) {
    l = (l << 4) + (unsigned char)c;
    ++tmp;
  }

  *dest = l;
  return (size_t)(tmp - src);
}
