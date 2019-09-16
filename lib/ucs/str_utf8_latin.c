#include "../ucs.h"
#include "../str.h"

size_t
str_utf8_latin1(char* s) {
  size_t i, r, w, len = str_len(s);
  for(r = 0, w = 0; s[r]; r += i) {
    i = scan_latin1_utf8(&s[r], len - r, &s[w]);
    ++w;
  }
  s[w] = '\0';
  return w;
}
