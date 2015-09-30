#include "word.h"

/* word_fill sets the words out[0], out[1], ..., out[len - 1] to c */
void word_fill(void* out, size_t len, int c) {
  register short* s = out;
  register const short* t = s + len;
  for(;;) {
    if(s == t) break; *s = c; ++s;
    if(s == t) break; *s = c; ++s;
    if(s == t) break; *s = c; ++s;
    if(s == t) break; *s = c; ++s;
  }
}
