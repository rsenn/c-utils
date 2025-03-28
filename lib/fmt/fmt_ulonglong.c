#include "../fmt.h"
#include "../uint64.h"

size_t
fmt_ulonglong(char* dest, uint64 i) {
  size_t len;
  uint64 tmp, len2;
  /* first count the number of bytes needed */

  for(len = 1, tmp = i; tmp > 9; ++len)
    tmp /= 10;

  if(dest)

    for(tmp = i, dest += len, len2 = len + 1; --len2; tmp /= 10)
      *--dest = (tmp % 10) + '0';
  return len;
}
