#include "word.h"

/* word_copyr copies in[len - 1] to out[len - 1], in[len - 2] to out[len - 2],
 * ... and in[0] to out[0] */
void word_copyr(void* out, size_t len, const void* in) {
  register short* s = (short *)out + len;
  register const short* t = in;
  register const short* u = t + len;
  for(;;) {
    if(t >= u) break; --u; --s; *s = *u;
    if(t >= u) break; --u; --s; *s = *u;
    if(t >= u) break; --u; --s; *s = *u;
    if(t >= u) break; --u; --s; *s = *u;
  }
}
