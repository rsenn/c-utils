#include "../ihex.h"
#include "../uint32.h"
#include "../alloc.h"
#include <assert.h>

ihex_record*
ihex_record_insert(ihex_file* ihf, uint32 at, uint8 len) {
  ihex_record* r = 0;
  ihex_recptr p;
  ihex_addr o, prev = {0};
  o.off32 = 0;
  /* for(rp = &ihf->records; *rp; rp = &(*rp)->next) {
     r = *rp;
     if(r->type == 4) {
       uint16_unpack_big(r->data, &o.hi16);
       o.lo16 = 0;
     } else {
       o.lo16 = r->offset;
     }
     if(at < o.off32) {
       break;
     }
     prev = o;
     prev.off32 += r->length;
   }*/
  p = ihex_record_find(ihf, at, &o.off32) /*: &ihf->records*/;

  if(p.link) {
    assert(at + len <= o.off32);
  }
  assert(at >= prev.off32);
  if((at & 0xffff0000) != (prev.off32 & 0xffff0000)) {
    r = (ihex_record*)alloc(sizeof(ihex_record) + 2);
    r->type = 4;
    r->length = 2;
    uint16_pack_big(r->data, at >> 16);
    r->checksum = ihex_record_checksum(r);
    list_add(&r->link, p.link);
  }
  r = (ihex_record*)alloc_zero(sizeof(ihex_record) + len);

  r->offset = at & 0xffff;
  r->length = len;
  list_add(&r->link, p.link);
  return r;
}
