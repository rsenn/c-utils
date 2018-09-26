#include "stralloc.h"
#include "byte.h"

void stralloc_remove_all(stralloc *sa, register const char *delchars, register unsigned int delcharslen)
{
  register unsigned int i;
  register char *x;
  register char *t;

  t = x = sa->s;
  if (!x) return;  /* safety */
  i = sa->len;
  while (i) {
    if (byte_chr(delchars, delcharslen, *x) == delcharslen) {  /* good char found */
      *t++ = *x;
    }
    x++;
    i--;
  }
  sa->len = (unsigned int)(t - sa->s);
}
