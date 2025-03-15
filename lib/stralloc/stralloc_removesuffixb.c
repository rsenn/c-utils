#include "../stralloc.h"

int
stralloc_removesuffixb(stralloc* sa, const char* x, size_t n) {
  if(n && stralloc_endb(sa, x, n)) {
    sa->len -= n;
    return 1;
  }

  return 0;
}
