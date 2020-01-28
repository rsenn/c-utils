#include "../stralloc.h"
#include "../byte.h"

size_t
stralloc_decamelize(const char* s, stralloc* sa, char sep) {
  const char* x = s;
  while(*x) {
    stralloc_catb(sa, x, 1);

    if(x[0] >= 'a' && x[0] <= 'z' && x[1] >= 'A' && x[1] <= 'Z') {
      stralloc_catc(sa, sep);
      stralloc_catc(sa, x[1] + 0x20);
      ++x;
    }

    x++;
  }
  return sa->len;
}
