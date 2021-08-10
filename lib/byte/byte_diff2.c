#include "../byte.h"

/* str_diff returns negative, 0, or positive, depending on whether the
 * string a[0], a[1], ..., a[n]=='\0' is lexicographically smaller than,
 * equal to, or greater than the string b[0], b[1], ..., b[m-1]=='\0'.
 * When the strings are different, str_diff does not read bytes past the
 * first difference. */
int
byte_diff2(const char* a, size_t alen, const char* b, size_t blen) {

  if(alen < blen)
    return -b[alen];

  if(blen < alen)
    return a[blen];

  return byte_diff(a, alen, b);
}
