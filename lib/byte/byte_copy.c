#include "../byte.h"
#include <string.h>

/* byte_copy copies in[0] to out[0], in[1] to out[1], ... and in[len - 1]
 * to out[len - 1]. It copies from the front, so when out lies inside the
 * range of in the bytes already written are copied again (what an
 * overlapping memmove does not do); everything else is memcpy or memmove. */
void
byte_copy(void* out, size_t len, const void* in) {
  char* s = (char*)out;
  const char* t = (const char*)in;
  size_t i;

  if(s > t && s < t + len) {
    for(i = 0; i < len; ++i)
      s[i] = t[i];
  } else {
    memmove(s, t, len);
  }
}
