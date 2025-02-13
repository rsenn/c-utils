#include "../utf8.h"
#include <string.h>

size_t
u8s_chrs(const char* in, const char needles[], size_t nn) {
  size_t i;

  for(i = 0; in[i];) {
    const size_t len = u8_len(&in[i], 1);

    for(size_t j = 0; j < nn;) {
      const size_t nlen = u8_len(&needles[j], 1);

      if(!memcmp(&in[i], &needles[j], nlen))
        return i;

      j += nlen;
    }

    i += len;
  }

  return i;
}
