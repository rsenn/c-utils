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

      /* only a *real* component (not "." or "..") can be cancelled
       * against a following ".." -- "../.." must stay "../..", not
       * collapse to "" (and a leading run of N ".." components must
       * survive intact, not get eliminated pairwise down to N%2). */
      if(l + 2 <= n && x[l] == '.' && x[l + 1] == '.' && (l + 2 >= n || x[l + 2] == sep) &&
         !(j - i == 2 && x[i] == '.' && x[i + 1] == '.') && !(j - i == 1 && x[i] == '.')) {
        l += 3;

        if(l > n)
          l = n;

      move:

        if(l < n)
          memmove(&x[i], &x[l], n - l);

        n = i + (n - l);
        x[n] = '\0';

        while(i > 0 && x[i - 1] == sep)
          --i;

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
