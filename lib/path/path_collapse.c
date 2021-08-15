#include "../path_internal.h"
#include "../byte.h"
#include <string.h>
#include <stdlib.h>

size_t
path_collapse(char* path, size_t n) {
  char *x, *end;
  int ret = 0;
  char sep = path_getsep(path);
  size_t l, i;

  for(x = path, end = path + n, i = 0; i < n;) {
    while(x[i] == sep)
      i++;

    l = i + byte_chr(&x[i], n - i, sep);
    if(l < n) {
      l++;
      if(l + 2 <= n) {
        if(x[l] == '.' && x[l + 1] == '.' && (l + 2 >= n || x[l + 2] == sep)) {
          l += 3;
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
      }
    }

    i = l;
  }

  return n;
}
