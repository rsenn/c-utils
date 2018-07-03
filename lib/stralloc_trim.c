#include "stralloc.h"
#include "byte.h"

void stralloc_trim(register stralloc *sa, register const char *trimchars, register unsigned int trimcharslen)
{
  if(!sa->s) return;
  while(sa->len > 0) {
    if(byte_chr(trimchars, trimcharslen, sa->s[sa->len - 1]) == trimcharslen) return;
    --sa->len;
  }
}
