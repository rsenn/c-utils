#include "../ihex.h"
#include "../scan.h"
#include "../alloc.h"

ssize_t
ihex_record_read(ihex_record** pihr, const char* in, size_t n) {
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
  x += i;
  n -= i;
  if((i = scan_xshort(x, &off)) != 4)
    return 0;
  x += i;
  n -= i;
  if((i = scan_xchar(x, &typ)) != 2)
    return 0;
  x += i;
  n -= i;
  if((i = scan_xchar(&x[(len * 2)], &chk)) != 2)
    return 0;

  if((*pihr = (ihex_record*)alloc(sizeof(ihex_record) + len))) {
    size_t j;
    ihex_record* r = *pihr;
    uint8* data = (uint8*)&r->data;

    for(j = 0; j < len; j++) {
      if((i = scan_xchar(x, &data[j])) != 2) {
        free(*pihr);
        *pihr = 0;
        return 0;
      }

      x += i;
      n -= i;
    }

    r->next = 0;
    r->length = len;
    r->type = typ;
    r->offset = off;
    r->checksum = chk;

    return x - in;
  }
  return 0;
}
