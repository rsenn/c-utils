#include "../utf8.h"
#include <string.h>

int
u8s_diff(const char* a, const char* b) {
  int j, alen, blen;
  for(;; a += alen, b += blen) {
    wchar_t achr = 0, bchr = 0;

    alen = u8towc(&achr, a);
    blen = u8towc(&bchr, b);

    if((j = (achr - bchr)))
      break;

    if(!bchr)
      break;
  }

  return j;
}
