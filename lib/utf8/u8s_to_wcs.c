#include "../utf8.h"

/* Converts the UTF-8 string u8 to at most count wide characters in out, a
 * terminating L'\0' is stored if there is room for it. With out 0 it returns
 * the length that the string would have. Returns the number of characters
 * stored, or (size_t)-1 if the string is not valid UTF-8. */
size_t
u8s_to_wcs(wchar_t* out, const char* u8, size_t count) {
  size_t clen = 0;

  if(NULL == out)
    return u8s_len(u8);

  while(*u8 && clen < count) {
    const size_t ulen = u8_to_wc(&out[clen], u8);

    if(ulen == 0)
      return (size_t)-1;

    clen++;
    u8 += ulen;
  }

  if('\0' == *u8 && clen < count)
    out[clen++] = L'\0';

  return clen;
}
