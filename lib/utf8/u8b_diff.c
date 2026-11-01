#include "../utf8.h"

/* Compares the characters of the n1 bytes at x1 with those of the n2 bytes at
 * x2: the difference of the first characters that differ, or if one is a
 * prefix of the other 0 when they have the same length and otherwise the
 * difference of the lengths in bytes (negative if x1 is the shorter). A byte
 * that is not valid UTF-8 counts as the character with its value. */
int
u8b_diff(const char* x1, size_t n1, const char* x2, size_t n2) {
  while(n1 && n2) {
    wchar_t c1, c2;
    size_t l1 = u8_to_wc(&c1, x1), l2 = u8_to_wc(&c2, x2);

    if(l1 == 0 || l1 > n1) {
      c1 = (unsigned char)*x1;
      l1 = 1;
    }
    if(l2 == 0 || l2 > n2) {
      c2 = (unsigned char)*x2;
      l2 = 1;
    }

    if(c1 != c2)
      return (int)c1 - (int)c2;

    x1 += l1;
    n1 -= l1;
    x2 += l2;
    n2 -= l2;
  }

  return n1 == n2 ? 0 : n1 ? 1 : -1;
}
