#include "../byte.h"
#include "../stralloc.h"

void
stralloc_remove_all(stralloc* sa, register const char* delchars, register unsigned int delcharslen) {
  register size_t i;
  register char *x, *t;

  t = x = sa->s;

  if(!x)
    return; /* safety */
  i = sa->len;

  while(i) {
    if(byte_chr(delchars, delcharslen, *x) == delcharslen) { /* good char found */
      *t++ = *x;
    }
    x++;
    i--;
  }
  sa->len = (size_t)(t - sa->s);
}
