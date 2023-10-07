#include "../byte.h"
#include "../stralloc.h"

void
stralloc_catq(stralloc* sa, const char* x, size_t len, const char q[]) {
  size_t j, k;
  if(q[0] == '\0') {
    stralloc_catb(sa, x, len);
    return;
  }
  for(j = 0; j < len;) {
    k = byte_chr(&x[j], len - j, ' ');
    if(k) {
      if(j)
        stralloc_catc(sa, ' ');
      stralloc_cats(sa, q);
      stralloc_catb(sa, &x[j], k);
      stralloc_cats(sa, q);
    }
    j += k + 1;
  }
}
