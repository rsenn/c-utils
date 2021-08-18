#include "../ihex.h"
#include "../uint32.h"
#include "../alloc.h"
#include <assert.h>

ihex_record*
ihex_record_insert(ihex_file* ihf, uint32 at, uint8 len) {
  ihex_recptr p;
  ihex_addr o = {0}, prev = {0};
  uint32 end = at + len;

  list_for_each(p.link, &ihf->records) {
    if(ihex_record_address(p.rec, &o) == 4)
      continue;

    if(o.off32 >= at)
      break;

    prev = o;
  }

  if(p.link) {
    assert(at + len <= o.off32);
  }
  ihex_record* r;

  if((at & 0xffff0000) != (prev.off32 & 0xffff0000)) {
    r = (ihex_record*)alloc(sizeof(ihex_record) + 2);
    r->type = 4;
    r->length = 2;
    uint16_pack_big(r->data, at >> 16);
    r->checksum = ihex_record_checksum(r);

    list_add_tail(&r->link, p.link);
    p.link = &r->link;
  }
  r = (ihex_record*)alloc_zero(sizeof(ihex_record) + len);

  r->offset = at & 0xffff;
  r->length = len;
  list_add_tail(&r->link, p.link);
  return r;
}
