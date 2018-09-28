#include "../stralloc.h"
#include "../utf8.h"

extern int
stralloc_copywcs(stralloc* sa, const wchar_t* buf) {
  return stralloc_copywb(sa, buf, wcslen(buf));
}

