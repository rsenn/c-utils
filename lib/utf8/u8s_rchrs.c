#include "../utf8.h"
#include <string.h>

size_t
u8s_rchrs(const char* u8, const char needles[], size_t nn) {
  int found = 0;
  size_t i, pos;

  for(i = 0; u8[i];) {
    const size_t len = u8_len(&u8[i], 1);

    for(size_t j = 0; j < nn;) {
      const size_t nlen = u8_len(&needles[j], 1);

      if(!memcmp(&u8[i], &needles[j], nlen)) {
        pos = i;
        found = 1;
      }

      j += nlen;
    }

    i += len;
  }

  return found ? pos : i;
}
