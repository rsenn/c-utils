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
          if(d > dest)
            *d++ = ' ';
      }
      if(len == 0) break;

      j = scan_nonwhitenskip(s, len);

      while(j > 0) {
          *d++ = *s++;
          --len;
          --j;
      }
      
      if(len == 0) break;
  }
  return d - dest;
}

