#undef __dietlibc__
#include "wstr.h"

size_t wstr_len(const wchar_t* in) {
  register const wchar_t* t = in;
  for(;;) {
    if(!*t) break; ++t;
    if(!*t) break; ++t;
    if(!*t) break; ++t;
    if(!*t) break; ++t;
  }
  return t - in;
}
