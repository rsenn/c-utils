#include "../scan.h"
#include "../str.h"

size_t
scan_xlongn(const char* src, size_t n, unsigned long* dest) {
  const char* tmp = src;
  unsigned long l = 0;
  int c;

  while(n-- > 0 && (l >> (sizeof(l) * 8 - 4)) == 0 && str_has("0123456789abcdefABCDEF", *tmp) && (c = scan_fromhex((unsigned char)*tmp)) >= 0) {
    l = (l << 4) + (unsigned char)c;
    ++tmp;
  }

  *dest = l;
  return (size_t)(tmp - src);
}
