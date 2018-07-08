#include "../byte.h"
#include "../str.h"
/*
  return index to first occurance of data,
  otherwise return str_len(s)
*/
unsigned int str_findb(const void *p1, const void *what, unsigned int len)
{
  register unsigned int i;
  register unsigned int s_len;
  register unsigned int last;
  register const char *s;
  s = p1;
  s_len = str_len(s);
  if(s_len < len) return s_len;
  last = s_len - len;
  for(i = 0; i <= last; i++) {
    if(byte_equal(s, len, what)) return i;
    s++;
  }
  return s_len;
}
