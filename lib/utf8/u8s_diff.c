#include "../utf8.h"
#include <string.h>

int
u8s_diff(const char* s1, const char* s2) {
  int r;

  for(;;) {
    wchar_t c1 = 0, c2 = 0;
    const int l1 = u8towc(&c1, s1), l2 = u8towc(&c2, s2);

    if((r = (c1 - c2)))
      break;

    if(!c2)
      break;

    s1 += l1;
    s2 += l2;
  }

  return r;
}
