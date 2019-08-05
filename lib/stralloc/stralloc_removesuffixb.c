#include "../stralloc.h"

int
stralloc_removesuffixb(stralloc* sa, const char* x, size_t len) {
  if(len && stralloc_endb(sa, x, len)) {
    sa->len -= len;
    return 1;
  }
  return 0;
}

