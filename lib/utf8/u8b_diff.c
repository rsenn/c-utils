#include "../utf8.h"
#include <string.h>

int
u8b_diff(const char* x1, size_t n1, const char* x2, size_t n2) {
  for(;;) {
    wchar_t c1 = 0, c2 = 0;
    int j, l1 = u8towc(&c1, x1), l2 = u8towc(&c2, x2);

    if((j = (c1 - c2)))
      return j;

    x1 += l1;
    n1 -= l1;

    x2 += l2;
    n2 -= l2;

    if(!n2)
      break;
  }

  return 0;
}
