#include "../str.h"

size_t
str_rfindb(const char* s1, const char* x, size_t n) {
  register const char *s, *b;
  size_t len = str_len(s1);

  if(n == 0 || !*x)
    return len;

  for(s = s1 + len - n; s >= s1; s--) {
    size_t i;
    for(i = 0, b = s; x[i] == *b++;) {
      if(++i == n)
        return s - s1;
    }
  }
  return len;
}
