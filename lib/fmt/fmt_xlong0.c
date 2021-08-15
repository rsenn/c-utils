#include "../fmt.h"

size_t
fmt_xlong0(char* dest, size_t num, size_t n) {
  size_t i = 0, len, tmp;
  len = fmt_xlong(NULL, num);
  if(len < n) {
    len = n - len;
    while(i < len)
      dest[i++] = '0';
  }
  i += fmt_xlong(&dest[i], num);
  return i;
}
