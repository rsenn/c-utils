#include "word.h"

/* word_chr returns the smallest integer i between 0 and len - 1
 * inclusive such that one[i] equals needle, or len if not found. */
size_t word_chr(const void* haystack, size_t len, short needle) {
  register short c = needle;
  register const short* s = haystack;
  register const short* t = s + len;
  for(;;) {
    if(s == t) break; if(*s == c) break; ++s;
    if(s == t) break; if(*s == c) break; ++s;
    if(s == t) break; if(*s == c) break; ++s;
    if(s == t) break; if(*s == c) break; ++s;
  }
  return s - (const short *)haystack;
}
