#include "../utf8.h"
#include "../byte.h"

/* Converts the wide string in to at most count bytes of UTF-8 in u8. A
 * character that has no encoding is written as the entity "&#x00;", which is
 * what wcs_u8slen counts for it. The result is terminated if there is room.
 * With u8 0 it returns the length that the conversion needs. Returns the
 * number of bytes stored, without the terminator. */
size_t
wcs_to_u8s(char* u8, const wchar_t* in, size_t count) {
  size_t clen = 0;
  wchar_t w;

  if(NULL == u8)
    return wcs_u8slen(in);

  while((w = *in++)) {
    const size_t ulen = wc_u8len(w);

    if(ulen > 0) {
      char tmp[5];

      if(clen + ulen > count)
        goto done;

      /* wc_to_u8 terminates what it writes, so it gets a place of its own */
      wc_to_u8(tmp, w);
      byte_copy(u8 + clen, ulen, tmp);
      clen += ulen;
    } else {
      if(clen + 6 > count)
        goto done;

      byte_copy(u8 + clen, 6, "&#x00;");
      clen += 6;
    }
  }

done:
  if(clen < count)
    u8[clen] = '\0';

  return clen;
}
