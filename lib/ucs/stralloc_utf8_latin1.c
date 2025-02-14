#include "../stralloc.h"
#include "../ucs.h"

size_t
stralloc_utf8_latin1(stralloc* sa) {
  size_t r, w;

  for(r = 0, w = 0; r < sa->len;) {
    r += scan_latin1_utf8(&sa->s[r], sa->len - r, &sa->s[w]);
    ++w;
  }

  sa->len = w;
  return w;
}
