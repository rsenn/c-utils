#include "../stralloc.h"
#include "../utf8.h"
#include <wchar.h>

extern int
stralloc_copywcs(stralloc* sa, const wchar_t* buf) {
  return stralloc_copywcb(sa, buf, wcslen(buf));
}
