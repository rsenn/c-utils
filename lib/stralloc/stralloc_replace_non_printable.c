#include "../stralloc.h"
#include <ctype.h>

void
stralloc_replace_non_printable(stralloc* sa, char ch) {
  unsigned int i;
  char c;

  for(i = 0; i < sa->len; ++i) {
    c = sa->s[i];
  
    if(!isprint(c)) 
      sa->s[i] = ch;
  }
}
