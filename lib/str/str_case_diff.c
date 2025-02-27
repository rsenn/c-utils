#include "../str.h"

int
str_case_diff(const void* p1, const void* p2) {
  int x, y;
  const char *s, *t;

  for(s = p1, t = p2;;) {
    if((x = *s++) >= 'A' && x <= 'Z') /* upper case */
      x += 'a' - 'A';

    if((y = *t++) >= 'A' && y <= 'Z') /* upper case */
      y += 'a' - 'A';

    if(x != y)
      break;

    if(!x || !y)
      break;
  }

  return x - y;
}
