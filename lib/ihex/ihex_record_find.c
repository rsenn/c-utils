#include "../ihex.h"
#include "../uint32.h"

ihex_recptr
ihex_record_find(ihex_file* ihf, uint32 at, uint32* roffs) {
  ihex_recptr p;
  ihex_addr o = {0};

  list_for_each(p.link, &ihf->records) {
    if(ihex_record_address(p.rec, &o) == 4)
      continue;

    o.lo16 = p.rec->offset;

    if(at >= o.off32 && at < o.off32 + p.rec->length) {
      if(roffs)
        *roffs = o.off32;
      break;
    }
  }
  return p;
}
