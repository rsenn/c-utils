#include "../stralloc.h"
#include <ctype.h>

int
stralloc_decamelize(stralloc* sa, stralloc* to) {
  size_t i;
  char prev = ' ';
  for(i = 0; i < sa->len; ++i) {
    char c = sa->s[i];

    if(islower(prev) && isupper(sa->s[i])) {
      stralloc_append(to, "_");
    }
    stralloc_catc(to, tolower(c));

    prev = sa->s[i];
  }
  return i;
}
