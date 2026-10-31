#include "../byte.h"
#include "../stralloc.h"

void
stralloc_remove_all(stralloc* sa, register const char* delchars, register unsigned int delcharslen) {
  size_t i;
  register char *x, *t;

  if(!(x = t = sa->s))
    return; /* safety */

  for(i = sa->len; i > 0; --i) {
    if(byte_chr(delchars, delcharslen, *x) == delcharslen) /* good char found */
      *t++ = *x;

    x++;
  }

  sa->len = (size_t)(t - sa->s);
}
