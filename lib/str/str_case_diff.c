#include "str.h"

int str_case_diff(const void *p1, const void *p2)
{
  register int x;
  register int y;
  register const char *s;
  register const char *t;
  s = p1;
  t = p2;
  for (;;) {
    x = *s++;
    if (x >= 'A' && x <= 'Z') {  /* upper case */
      x += 'a' - 'A';
    }
    y = *t++;
    if (y >= 'A' && y <= 'Z') {  /* upper case */
      y += 'a' - 'A';
    }
    if (x != y) break;
    if (!x) break;
    if (!y) break;
  }
  return x - y;
}
