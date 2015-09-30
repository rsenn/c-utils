#include "byte.h"
#include "wstralloc.h"

int wstralloc_insertb(wstralloc* sa, const wchar_t* s, size_t pos, size_t n) {
  if(pos >= sa->len)
    return wstralloc_catb(sa, s, n);
  if(!wstralloc_readyplus(sa, n))
    return 0;
  word_copyr(&sa->s[pos + n], sa->len - pos, &sa->s[pos]);
  word_copy(&sa->s[pos], n, s);
  sa->len += n;
  return n;
}
