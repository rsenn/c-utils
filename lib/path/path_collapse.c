#include "../path_internal.h"
#include "../byte.h"
#include <string.h>
#include <stdlib.h>

size_t
path_collapse(char* path, size_t n) {
  char *x = path, *end = path + n, sep = path_getsep(path);
  int count = 0;
  size_t l, i = 0, j = 0;

  while(i < n) {
    while(x[i] == sep)
      ++i;

    if((l = i + byte_chr(&x[i], n - i, sep)) < n) {
      j = l;

      while(x[l] == sep)
        ++l;

      if(l + 2 <= n && x[l] == '.' && x[l + 1] == '.' && (l + 2 >= n || x[l + 2] == sep)) {
        l += 3;

      move:

        if(l < n)
          memmove(&x[i], &x[l], n - l);

        n = i + (n - l);
        x[n] = '\0';

        while(x[--i] == sep)
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

          if(j < n && x[j] == sep)
            ++j;
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
