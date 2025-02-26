#include "../byte.h"

/* byte_chr returns the smallest integer i between 0 and len - 1
 * inclusive such that one[i] equals needle, or len if not found. */
size_t
byte_chrs(const char* haystack, size_t len, const char needle[], size_t nl) {
  const char *s, *end;

  for(s = haystack, end = haystack + len; s != end; ++s)

    if(byte_chr(needle, nl, *s) < nl)
      break;

  return s - (const char*)haystack;
}
