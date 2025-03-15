#include "../byte.h"

size_t
byte_chrns(const char* x, size_t n, const char charset[], size_t setlen) {
  const char *s, *end;

  for(s = x, end = x + n; s != end; ++s)
    if(byte_chr(charset, setlen, *s) == setlen)
      break;

  return s - x;
}
