#include "../byte.h"

/* str_diff returns negative, 0, or positive, depending on whether the
 * string a[0], a[1], ..., a[n]=='\0' is lexicographically smaller than,
 * equal to, or greater than the string b[0], b[1], ..., b[m-1]=='\0'.
 * When the strings are different, str_diff does not read bytes past the
 * first difference. */
int
str_diff(const char* a, const char* b) {
  int j;

  for(;; ++a, ++b) {
    if((j = (*a - *b)))
      break;

    if(!*b)
      break;
  }

  return j;
}
