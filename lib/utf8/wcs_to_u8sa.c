#include "../utf8.h"
#include "../stralloc.h"

size_t
wcs_to_u8sa(stralloc* sa, const wchar_t* in) {
  size_t clen = 0;
  wchar_t w;
  const int len = wcs_u8slen(in);

  if(NULL == sa)
    return (size_t)len;

  while((w = *in++)) {
    const int ulen = wc_u8len(w);

    if(ulen >= 0) {
      if(!stralloc_readyplus(sa, ulen))
        break;
      clen += wc_to_u8(&sa->s[sa->len], w);
    } else {
      if(!stralloc_cats(sa, "&#x00;"))
        break;
    }
  }

  return (size_t)clen;
}
