#include "../stralloc.h"
#include "../utf8.h"

extern int
stralloc_catwcs(stralloc* sa, const wchar_t* buf) {
  return stralloc_catwcb(sa, buf, wcslen(buf));
}
