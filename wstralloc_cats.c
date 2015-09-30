#include "wstralloc.h"
#include "wstr.h"

extern int wstralloc_cats(wstralloc* sa, const wchar_t* buf) {
  return wstralloc_catb(sa, buf, wstr_len(buf));
}

