#include "byte.h"

/* wstr_diff returns negative, 0, or positive, depending on whether the
 * wstring a[0], a[1], ..., a[n] == '\0' is lexicographically smaller than,
 * equal to, or greater than the wstring b[0], b[1], ..., b[m - 1] == '\0'.
 * When the wstrings are different, wstr_diff does not read bytes past the
 * first difference. */
int wstr_diffn(const wchar_t* a, const wchar_t* b, size_t limit) {
  register const unsigned short* s = (const unsigned short *)a;
  register const unsigned short* t = (const unsigned short *)b;
  register const unsigned short* u = t + limit;
  register int j;
  j = 0;
  for(;;) {
    if(t >= u) break; if((j = (*s - *t))) break; if(!*t) break; ++s; ++t;
    if(t >= u) break; if((j = (*s - *t))) break; if(!*t) break; ++s; ++t;
    if(t >= u) break; if((j = (*s - *t))) break; if(!*t) break; ++s; ++t;
    if(t >= u) break; if((j = (*s - *t))) break; if(!*t) break; ++s; ++t;
  }
  return j;
}
