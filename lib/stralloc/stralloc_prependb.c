#include "../byte.h"
#include "../stralloc.h"

int
stralloc_prependb(stralloc* sa, const char* s, size_t n) {
  if(stralloc_readyplus(sa, n)) return 0;
  byte_copyr(&sa->s[n], sa->len, &sa->s);
  byte_copy(sa->s, n, s);
  sa->len += n;
  return n;
}
