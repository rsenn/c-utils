#include "../stralloc.h"
#include "../byte.h"
#include <assert.h>

int
stralloc_replace(stralloc* sa, size_t pos, size_t len, const char* to, size_t tolen) {
  char* x;
  size_t n;

  assert(pos + len <= sa->len);

  if(tolen > len) {
    if(!stralloc_readyplus(sa, tolen - len))
      return 0;
  }

  x = &sa->s[pos];
  n = sa->len - pos;

  if(len < tolen) {
    byte_copyr(x + tolen, n - len, x + len);
    sa->len += tolen - len;
  } else if(len > tolen) {
    byte_copy(x + tolen, n - len, x + len);
    sa->len -= len - tolen;
  }

  if(tolen)
    byte_copy(x, tolen, to);

  return 1;
}
