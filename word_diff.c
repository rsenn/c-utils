#include "word.h"

/* word_diff returns negative, 0, or positive, depending on whether the
 * string one[0], one[1], ..., one[len - 1] is lexicographically smaller
 * than, equal to, or greater than the string one[0], one[1], ...,
 * one[len - 1]. When the strings are different, word_diff does not read
 * words past the first difference. */
int word_diff(const void* a, size_t len, const void* b) {
#if 0
  /* this gets miscompiled by gcc 4.3.2 on x86_64 */
  register const unsigned short* s = a;
  register const unsigned short* t = b;
  register const unsigned short* u = t + len;
  register int j;
  j = 0;
  for(;;) {
    if(t == u) break; if((j = ((unsigned int) * s - *t))) break; ++s; ++t;
    if(t == u) break; if((j = ((unsigned int) * s - *t))) break; ++s; ++t;
    if(t == u) break; if((j = ((unsigned int) * s - *t))) break; ++s; ++t;
    if(t == u) break; if((j = ((unsigned int) * s - *t))) break; ++s; ++t;
  }
  return j;
#else
  size_t i;
  for(i = 0; i < len; ++i) {
    int r = ((unsigned short *)a)[i] - ((unsigned short *)b)[i];
    if(r) return r;
  }
  return 0;
#endif
}
