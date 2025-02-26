#include "../ihex.h"
#include "../uint32.h"
#include "../alloc.h"
#include <assert.h>

ihex_record*
ihex_record_insert(ihex_file* ihf, uint32 at, uint8 len) {
  ihex_recptr p;
  ihex_addr o = {0};
  uint32 prevAddr = 0, end = at + len;
  ihex_record *r = 0, *prev = 0;

  if(list_empty(&ihf->records)) {
    p.link = &ihf->records;
  } else {
    list_for_each(p.link, &ihf->records) {
      ihex_record_address(p.rec, &o);

      if(o.off32 > at)
        break;
      prevAddr = o.off32;
      prev = p.rec;
    }

    if(p.link == 0) {
      p.link = &ihf->records;
    } else {
      if(o.off32 >= at && o.off32 < at + len) {
        buffer_puts(buffer_2, "Overlap ");
        buffer_putxlong0(buffer_2, o.off32, 4);
        buffer_puts(buffer_2, "  ");
        buffer_putxlong0(buffer_2, o.off32 + p.rec->length, 4);
        buffer_puts(buffer_2, " : ");
        buffer_putxlong0(buffer_2, at, 4);
        buffer_puts(buffer_2, " - ");
        buffer_putxlong0(buffer_2, at + len, 4);
        buffer_putnlflush(buffer_2);
        return 0;
      }
    }
  }

  if((at & 0xffff0000) != (prevAddr & 0xffff0000)) {
    r = (ihex_record*)alloc(sizeof(ihex_record) + 2);
    r->type = 4;
    r->length = 2;
    uint16_pack_big((char*)r->data, at >> 16);
    r->checksum = ihex_record_checksum(r);

    list_add_tail(&r->link, p.link);
    p.link = r->link.next;
  }
  r = (ihex_record*)alloc_zero(sizeof(ihex_record) + len);

  r->offset = at & 0xffff;
  r->length = len;

  list_add_tail(&r->link, p.link);
  return r;
}
