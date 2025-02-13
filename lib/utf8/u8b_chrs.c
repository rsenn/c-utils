#include "../utf8.h"
#include <string.h>

size_t
u8b_chrs(const char* in, size_t n, const char needles[], size_t nn) {
  for(size_t i = 0; i < n;) {
    size_t len = u8len(&in[i], 1);

    for(size_t j = 0; j < nn;) {
      size_t nlen = u8len(&needles[j], 1);

      if(!memcmp(&in[i], &needles[j], nlen))
        return i;

      j += nlen;
    }

    i += len;
  }

  return n;
}
