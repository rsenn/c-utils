#include "../fmt.h"
#include "../str.h"
#include "../byte.h"

size_t
fmt_repeat(char* dest, const char* src, int n) {
  size_t i = 0, slen = str_len(src);
  while(n--) {
    byte_copy(&dest[i], slen, src);
    i += slen;
  }
  return i;
}
