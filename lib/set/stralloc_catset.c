#include "../set.h"
#include "../str.h"
#include "../stralloc.h"

size_t
stralloc_catset(stralloc* sa, const set_t* set, const char* separator) {
  size_t n;
  n = fmt_set(0, set, separator);
  stralloc_readyplus(sa, n);
  sa->len += (n = fmt_set(stralloc_end(sa), set, separator));
  return n;
}
