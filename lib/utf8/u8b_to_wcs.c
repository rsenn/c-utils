#include "../utf8.h"

size_t
u8b_to_wcs(wchar_t* out, const char* u8, size_t n, size_t count) {
  size_t i = 0, clen = 0;

  if(NULL == out)
    return u8b_len(u8, n);

  while(i < n && clen < count) {
    const size_t ulen = u8_to_wc(&out[clen], &u8[i]);

    if(ulen < 0)
      return -1;

    clen++;
    i += ulen;
  }

  if(i == n && clen < count)
    out[clen++] = L'\0';

  return clen;
}
