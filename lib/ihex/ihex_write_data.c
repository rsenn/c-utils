#include "../ihex.h"
#include "../byte.h"

/* ihex_write_data writes x[n] as data records for the address addr, at
 * most reclen bytes each and none across a 64k boundary. *hi16 holds the
 * extended linear address in effect, an extended linear address record is
 * written before the data when it changes. Start with *hi16 = 0. */
void
ihex_write_data(buffer* b, uint16* hi16, uint32 addr, const uint8* x, size_t n, unsigned reclen) {
  ihex_record* r;
  char tmp[sizeof(ihex_record) + 255];
  size_t len;

  if(!reclen || reclen > 255)
    reclen = 16;

  r = (ihex_record*)tmp;

  while(n) {
    len = n < reclen ? n : reclen;

    if(len > 0x10000u - (addr & 0xffffu))
      len = 0x10000u - (addr & 0xffffu);

    if((uint16)(addr >> 16) != *hi16) {
      *hi16 = (uint16)(addr >> 16);
      byte_zero(r, sizeof(ihex_record));
      r->length = 2;
      r->type = IHEX_EXTLINADDR;
      uint16_pack_big((char*)r->data, *hi16);
      r->checksum = ihex_record_checksum(r);
      ihex_record_write(r, b);
    }

    byte_zero(r, sizeof(ihex_record));
    r->length = (uint8)len;
    r->offset = (uint16)addr;
    r->type = IHEX_DATA;
    byte_copy(r->data, len, x);
    r->checksum = ihex_record_checksum(r);
    ihex_record_write(r, b);

    addr += (uint32)len;
    x += len;
    n -= len;
  }
}
