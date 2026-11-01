#include "../utf8.h"
#include "../stralloc.h"

/* Appends the wide string in to sa as UTF-8, characters without an encoding as
 * "&#x00;". With sa 0 it returns the length that this takes. Returns the
 * number of bytes appended, or 0 if the memory ran out (sa is kept as far as
 * it got). */
size_t
wcs_to_u8sa(stralloc* sa, const wchar_t* in) {
  size_t clen = 0;
  wchar_t w;

  if(NULL == sa)
    return wcs_u8slen(in);

  while((w = *in++)) {
    const size_t ulen = wc_u8len(w);

    if(ulen > 0) {
      char tmp[5];

      wc_to_u8(tmp, w);

      if(!stralloc_catb(sa, tmp, ulen))
        return 0;

      clen += ulen;
    } else {
      if(!stralloc_cats(sa, "&#x00;"))
        return 0;

      clen += 6;
    }
  }

  return clen;
}
