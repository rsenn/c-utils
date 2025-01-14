#include "../path_internal.h"
#include "../byte.h"
#include <string.h>
#include <stdlib.h>

size_t
path_collapse(char* path, size_t n) {
  char *x, *end, sep = path_getsep(path);
  int count = 0;
  size_t l, i, j = 0;

  for(x = path, end = path + n, i = 0; i < n;) {
    while(i < n && x[i] == sep)
      ++i;

    if((l = i + byte_chr(&x[i], n - i, sep)) < n) {
      j = l;

      while(l < n && x[l] == sep)
        ++l;

      if(l + 2 < n && x[l] == '.' && x[l + 1] == '.' && (l + 2 >= n || x[l + 2] == sep)) {
        l += 3;

      move:
        if(l < n)
          memmove(&x[i], &x[l], n - l);

        n = i + (n - l);
        x[n] = '\0';

        while(i > 0 && x[--i] == sep)
          ;
        while(i > 0 && x[i] != sep)
          i--;

        continue;
      }

      if(l + 1 <= n && x[l] == '.') {
        if(l + 1 >= n) {
          x[j] = '\0';
          return j;
        }

        if(x[l + 1] == sep) {
          l += 2;
          i = j;
          goto move;
        }
      }
    }

    i = l;
    ++count;
  }

  return n;
}
