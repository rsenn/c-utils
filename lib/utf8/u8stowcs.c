#include "../utf8.h"

size_t
u8stowcs(wchar_t* out, const char* u8, size_t count) {
  size_t clen = 0;

  if(NULL == out)
    return u8swcslen(u8);

  while(*u8 && clen < count) {
    const int ulen = u8towc(&out[clen], u8);

    if(ulen < 0)
      return (size_t)-1;

    clen++;
    u8 += ulen;
  }

  if('\0' == *u8 && clen < count)
    out[clen++] = L'\0';

  return clen;
}
