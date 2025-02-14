#include "../byte.h"

size_t
byte_chrns(const char* x,
           size_t len,
           const char charset[],
           size_t setlen) {
  const char* s = x;
  const char* end = x + len;

  for(;;) {
    if(s == end)
      break;
    if(byte_chr(charset, setlen, *s) == setlen)
      break;
    ++s;
  }
  return s - x;
}
