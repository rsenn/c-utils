#include "wstralloc.h"
#include "wstr.h"

extern int wstralloc_copys(wstralloc* sa, const wchar_t* buf) {
  return wstralloc_copyb(sa, buf, wstr_len(buf));
}

