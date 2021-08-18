#include "../ihex.h"
#include "../uint32.h"

ihex_record**
ihex_record_find(ihex_file* ihf, uint32 at, uint32* roffs) {
  ihex_record **ptr, *r;
  ihex_addr o;
  o.off32 = 0;
  for(ptr = &ihf->records; r = *ptr; ptr = &(*ptr)->next) {
    if(r->type == 4) {
      uint16_unpack_big(r->data, &o.hi16);
      continue;
    }
    o.lo16 = r->offset;
    if(at >= o.off32 && at < o.off32 + r->length) {
      if(roffs)
        *roffs = o.off32;
      return ptr;
    }
  }
  return 0;
}
