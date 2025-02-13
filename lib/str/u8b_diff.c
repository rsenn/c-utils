#include "../utf8.h"
#include <string.h>

int
u8b_diff(const char* a, size_t na, const char* b, size_t nb) {
  for(;;) {
    wchar_t achr = 0, bchr = 0;
    int j, alen = u8towc(&achr, a), blen = u8towc(&bchr, b);

    if((j = (achr - bchr)))
      return j;

    a += alen;
    na -= alen;

    b += blen;
    nb -= blen;

    if(!nb)
      break;
  }

  return 0;
}
