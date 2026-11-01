#include "../utf8.h"
#include "../byte.h"

/* Like u8s_to_wcs for the n bytes at u8, which need no terminator. A sequence
 * that is cut off by the end of the bytes is invalid. Returns (size_t)-1 for
 * invalid input. */
size_t
u8b_to_wcs(wchar_t* out, const char* u8, size_t n, size_t count) {
  size_t i = 0, clen = 0;

  if(NULL == out)
    return u8b_len(u8, n);

  while(i < n && clen < count) {
    size_t ulen;
    char tmp[5];
    size_t avail = n - i < 4 ? n - i : 4;

    /* decode from a terminated copy so that nothing past n is read */
    byte_copy(tmp, avail, u8 + i);
    tmp[avail] = '\0';

    if(tmp[0] == '\0') {
      out[clen++] = L'\0';
      i++;
      continue;
    }

    ulen = u8_to_wc(&out[clen], tmp);

    if(ulen == 0)
      return (size_t)-1;

    clen++;
    i += ulen;
  }

  if(i == n && clen < count)
    out[clen++] = L'\0';

  return clen;
}
