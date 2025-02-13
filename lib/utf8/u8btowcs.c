#include "../utf8.h"

size_t
u8btowcs(wchar_t* pw, const char* pu, size_t n, size_t count) {
  size_t i = 0, clen = 0;

  if(NULL == pw)
    return u8bwcslen(pu, n);

  while(i < n && clen < count) {
    int ulen = u8towc(&pw[clen], pu[i], 1);

    if(ulen < 0)
      return (size_t)-1;

    clen++;
    i += ulen;
  }

  if(i == n && clen < count)
    pw[clen++] = L'\0';

  return clen;
}
