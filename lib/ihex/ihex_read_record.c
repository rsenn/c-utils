#include "../ihex.h"
#include "../scan.h"

ssize_t
ihex_read_record(ihex_record* ihr, const char* in, size_t n) {
  uint8 b;
  uint16 s;
  const char* x = in;
  size_t i;
  if(x[0] != ':')
    return 0;
  x += 1;
  n -= 1;
  if((i = scan_xchar(x, &b)) != 2)
    return 0;
  ihr->reclen = b;
  x += i;
  n -= i;
  if((i = scan_xshort(x, &s)) != 4)
    return 0;
  ihr->offset = s;
  x += i;
  n -= i;
  if((i = scan_xchar(x, &b)) != 2)
    return 0;
  ihr->rectyp = b;
  x += i + (ihr->reclen * 2);
  n -= i + (ihr->reclen * 2);
  if((i = scan_xchar(x, &b)) != 2)
    return 0;
  ihr->checksum = b;
  x += i;
  n -= i;
  return x - in;
}
