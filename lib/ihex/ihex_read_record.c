#include "../ihex.h"
#include "../scan.h"

ssize_t
ihex_read_record(ihex_record* ihr, const char* in, size_t n) {
  uint8 len, typ, chk;
  uint16 off;
  const char* x = in;
  size_t i;
  if(x[0] != ':')
    return 0;
  x += 1;
  n -= 1;
  if((i = scan_xchar(x, &len)) != 2)
    return 0;
  ihr->reclen = len;
  x += i;
  n -= i;
  if((i = scan_xshort(x, &off)) != 4)
    return 0;
  ihr->offset = off;
  x += i;
  n -= i;
  if((i = scan_xchar(x, &typ)) != 2)
    return 0;
  ihr->rectyp = typ;
  x += i + (ihr->reclen * 2);
  n -= i + (ihr->reclen * 2);
  if((i = scan_xchar(x, &chk)) != 2)
    return 0;
  ihr->checksum = chk;
  x += i;
  n -= i;
  return x - in;
}
