#include "../fmt.h"

size_t
fmt_xlong0(char* dest, size_t num, int n) {
  size_t i = 0, len, tmp;
  len = fmt_xlong(NULL, num);
  if(len < n) {
    n -= len;
    while(i < n) dest[i++] = '0';
  }
  i += fmt_xlong(&dest[i], num);
  return i;
}
