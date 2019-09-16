#include "../byte.h"

/* byte_chr returns the smallest integer i between 0 and len - 1
 * inclusive such that one[i] equals needle, or len if not found. */
size_t
byte_chrs(const void* haystack, size_t len, char needle[], size_t nl) {
  const char* s = haystack;
  const char* end = haystack + len;

  for(;;) {
    if(s == end) break;
    if(byte_chr(needle, nl, *s) < nl)
	    break;
    ++s;
  }
  return s - (const char*)haystack;
}

