#include "../strlist.h"

size_t
strlist_count(const strlist* sl) {
  const char* x;
  ssize_t n, p, l = 1;

  if(sl->sa.len == 0)
    return 0;

  x = sl->sa.s;
  n = sl->sa.len;

  while(n > 0) {
    p = byte_chr(x, n, sl->sep);

    if(p == n)
      break;

    if(p < n) {
      char c = x[p];

      if(c == sl->sep) {
        p++;

        if(p == n)
          break;
      }
    }

    l++;
    x += p;
    n -= p;
  }
  return l;
}
