#include "../stralloc.h"
#include <ctype.h>

void
stralloc_replace_non_printable(stralloc* sa, char ch) {
  size_t i;
  for(i = 0; i < sa->len; ++i) {
    char c = sa->s[i];

    if(!isprint(c))
      sa->s[i] = ch;
  }
}
