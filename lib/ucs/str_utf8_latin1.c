#include "../ucs.h"
#include "../str.h"

size_t
str_utf8_latin1(char* s) {
  size_t i, r, w, len = str_len(s);

  for(r = 0, w = 0; s[r]; r += i) {
    i = scan_latin1_utf8(&s[r], len - r, &s[w]);
    if(i == 0) { /* not valid UTF-8: keep the byte and go on */
      s[w] = s[r];
      i = 1;
    }
    ++w;
  }

  s[w] = '\0';
  return w;
}
