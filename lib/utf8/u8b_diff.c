#include "../utf8.h"
#include <string.h>

int
u8b_diff(const char* x1, size_t n1, const char* x2, size_t n2) {
  int r;

  for(;;) {
    wchar_t c1 = 0, c2 = 0;
    const int l1 = u8_to_wc(&c1, x1), l2 = u8_to_wc(&c2, x2);

    if((r = (c1 - c2)))
      return r;

    x1 += l1;
    n1 -= l1;

    x2 += l2;
    n2 -= l2;

    if(!n2)
      break;
  }

  return 0;
}
