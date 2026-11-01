#include "../utf8.h"

/* Compares two UTF-8 strings by character: the difference of the first
 * characters that differ, 0 if they are equal. A byte that is not valid
 * UTF-8 counts as the character with its value. */
int
u8s_diff(const char* s1, const char* s2) {
  for(;;) {
    wchar_t c1 = 0, c2 = 0;
    size_t l1 = u8_to_wc(&c1, s1), l2 = u8_to_wc(&c2, s2);

    if(l1 == 0) {
      c1 = (unsigned char)*s1;
      l1 = *s1 ? 1 : 0;
    }
    if(l2 == 0) {
      c2 = (unsigned char)*s2;
      l2 = *s2 ? 1 : 0;
    }

    if(c1 != c2)
      return (int)c1 - (int)c2;

    if(!c1)
      return 0;

    s1 += l1;
    s2 += l2;
  }
}
