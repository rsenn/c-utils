#include "../str.h"

size_t
str_findb(const char* s1, const char* x, size_t n) {
  const char* b;
  size_t i, j, len = str_len(s1);

  if(len >= n) {
    size_t end = len - n + 1;

    for(i = 0; i < end; i++) {
      b = &s1[i];

      for(j = 0; x[j] == b[j];) {
        if(++j == n)
          return i;
      }
    }
  }
  return len;
}
