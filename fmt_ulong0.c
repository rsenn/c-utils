#include "fmt.h"

unsigned int fmt_ulong0(char *dest,size_t i,unsigned int pad) {
  register unsigned int len;
  register size_t tmp;
  /* first count the number of bytes needed */
  for (len=1, tmp=i; tmp>9; ++len) tmp/=10;
  /* now see if we need to pad */
  if (dest) {
    while (len<pad) { *dest='0'; ++dest; ++len; }
    fmt_ulong(dest,i);
    return len;
  } else
    return (len<pad?pad:len);
}
