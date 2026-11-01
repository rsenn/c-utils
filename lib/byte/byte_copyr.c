#include "../byte.h"
#include <string.h>

/* byte_copyr copies in[len - 1] to out[len - 1], in[len - 2] to out[len - 2],
 * ... and in[0] to out[0]. When out lies before the end of in, inside its
 * range, the bytes already written are copied again. */
void
byte_copyr(void* out, size_t len, const void* in) {
  char* s = (char*)out + len;
  const char* t = (const char*)in;
  const char* u = t + len;

  if((char*)out < t && (char*)out + len > t) {
    for(;;) {
      if(t >= u)
        break;
      --u;
      --s;
      *s = *u;
    }
  } else {
    memmove(out, in, len);
  }
}
