#include "../case.h"

size_t
case_findb(const void* haystack, size_t hlen, const void* what, size_t wlen) {
  size_t i, last;
  const char* s = haystack;
  if(hlen < wlen) return hlen;
  last = hlen - wlen;
  for(i = 0; i <= last; i++) {
    if(!case_diffb(s, wlen, what)) return i;
    s++;
  }
  return hlen;
}

