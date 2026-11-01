#include "../fmt.h"
#include "../scan.h"
#include "../byte.h"

/* Leading and trailing white space is removed, every other run of white space
 * becomes one blank. Needs at most len bytes; dest may be NULL to measure
 * (it crashed on NULL before, and left a blank at the end). */
size_t
fmt_stripwhitespace(char* dest, const char* src, size_t len) {
  size_t n = 0, i, j;

  for(;;) {
    i = scan_whitenskip(src, len);
    src += i;
    len -= i;

    if(len == 0)
      break;

    if(i && n) {
      if(dest)
        dest[n] = ' ';
      ++n;
    }

    j = scan_nonwhitenskip(src, len);
    if(dest)
      byte_copy(dest + n, j, src);
    n += j;
    src += j;
    len -= j;
  }
  return n;
}
