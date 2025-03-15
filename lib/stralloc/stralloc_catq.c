#include "../byte.h"
#include "../stralloc.h"

void
stralloc_catq(stralloc* sa, const char* x, size_t n, const char q[]) {
  if(q[0] == '\0') {
    stralloc_catb(sa, x, n);
    return;
  }

  for(size_t j = 0; j < n;) {
    size_t k;

    if((k = byte_chr(&x[j], n - j, ' '))) {
      if(j)
        stralloc_catc(sa, ' ');

      stralloc_cats(sa, q);
      stralloc_catb(sa, &x[j], k);
      stralloc_cats(sa, q);
    }

    j += k + 1;
  }
}
