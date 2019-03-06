#include "../fmt.h"
#include "../scan.h"

size_t
fmt_stripwhitespace(char* dest, const char* src, size_t len) {
  register const char *s = src;
  register char *d = dest;
  size_t i, j;
  char c;

  for(;;) {
      i = scan_whitenskip(s, len);

      if(i) {
          s += i;
          len -= i;
          *dest++ = ' ';
      }

      j = scan_nonwhitenskip(s, len);

      while(i < j) {
          *d++ = *s++;
          --len;
      }
  }
  return d - dest;
}

