#include "../byte.h"
#include "../byte.h"
#include "../stralloc.h"
/*
  return index to first occurance of data,
  otherwise return sa->len
*/
unsigned int stralloc_findb(const stralloc *sa, const void *what, unsigned int len) {
  register unsigned int i;
  register unsigned int sa_len;
  register unsigned int last;
  register char *s;
  sa_len = sa->len;
  if(sa_len < len) return sa_len;
  last = sa_len - len;
  s = sa->s;
  for(i = 0; i <= last; i++) {
    if(byte_equal(s, len, what)) return i;
    s++;
  }
  return sa_len;
}
