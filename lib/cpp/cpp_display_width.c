#include "../utf8.h"

// Returns the number of columns needed to display a given
// string in a fixed-width font.
size_t
cpp_display_width(char* p, size_t len) {
  char *start = p, *end = p + len;
  size_t w = 0;

  while(p < end) {
    wchar_t c;
    p += u8_to_wc(&c, p);
    w += wc_charwidth(c);
  }

  return w;
}
