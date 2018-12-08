#include "../str.h"

#define lowerc(c) (((c) >= 'A' && (c) <= 'Z') ? (c) + ( 'a' - 'A') : (c))

size_t
case_findb(const char* s1, const char *x, size_t n) {
  register const char *s, *b;
  size_t i;

  if(n == 0 || !*x) return str_len(s1);

  for(s = s1; *s; s++) {
    for(i = 0, b = s; ; ) {
    unsigned int ch = *b++;
    if(lowerc(x[i]) != lowerc(ch)) break;
      if(++i == n)
        return s - s1;
    }
  }
  return s - s1;
}

