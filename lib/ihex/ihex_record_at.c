#include "../ihex.h"
#include "../uint32.h"

ihex_record*
ihex_record_at(ihex_file* ihf, uint32 at, uint32* roffs) {
  ihex_record* r;
  ihex_addr o;

  o.off32 = 0;

  for(r = ihf->records; r; r = r->next) {
    if(r->type == 4) {
      uint16_unpack_big(r->data, &o.hi16);
      continue;
    }

    o.lo16 = r->offset;

    if(at >= o.off32 && at < o.off32 + r->length) {
      if(roffs)
        *roffs = o.off32;
      return r;
    }
  }
  return 0;
}
