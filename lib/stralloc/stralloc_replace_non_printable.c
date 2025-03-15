#include "../stralloc.h"
#include <ctype.h>

void
stralloc_replace_non_printable(stralloc* sa, char ch) {
  for(size_t i = 0; i < sa->len; ++i) {
    char c = sa->s[i];

    if(!isprint(c))
      sa->s[i] = ch;
  }
}
