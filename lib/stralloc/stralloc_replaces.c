#include "../stralloc.h"
#include "../byte.h"
#include "../str.h"

int
stralloc_replaces(stralloc* sa, const char* from, const char* to) {
  char* x = sa->s;
  size_t tlen = str_len(to), flen = str_len(from), n = sa->len;

  for(size_t p = 0; p < sa->len;) {
    size_t i = byte_findb(&sa->s[p], sa->len - p, from, flen);
    p += i;

    if(p == sa->len)
      break;

    if(tlen > flen) {
      if(!stralloc_readyplus(sa, tlen - flen))
        return 0;
    }

    if(flen < tlen) {
      byte_copyr(&sa->s[p + tlen], sa->len - p - flen, &sa->s[p + flen]);
      sa->len += tlen - flen;
    } else if(flen > tlen) {
      byte_copy(&sa->s[p + tlen], sa->len - p - flen, &sa->s[p + flen]);
      sa->len -= flen - tlen;
    }

    if(tlen)
      byte_copy(&sa->s[p], tlen, to);

    p += tlen;
  }

  return 1;
}
