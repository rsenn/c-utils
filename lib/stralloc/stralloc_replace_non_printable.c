#include <stdint.h>
#include "../stralloc.h"
#include <ctype.h>

void stralloc_replace_non_printable(register stralloc *sa, char ch) {
  register unsigned int i;
  register char c;
  for(i = 0; i < sa->len; ++i) {
    c = sa->s[i];
    if(!isprint(c)) {
      sa->s[i] = ch;
    }
  }
}
