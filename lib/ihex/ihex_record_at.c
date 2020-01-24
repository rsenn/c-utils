#include "../ihex.h"
#include "../slist.h"
#include "../uint32.h"

ihex_record*
ihex_record_at(ihex_file* ihf, uint32 at, uint32* roffs) {
  ihex_record* rec;
  union {
    uint32 off32;
    struct {
      uint16 lo16;
      uint16 hi16;
    };
  } o;

  o.off32 = 0;

  slink_foreach(ihf->records, rec) {
    if(rec->type == 0) {
      o.lo16 = rec->offset;
    } else if(rec->type == 4) {
      o.hi16 = rec->data[1] | (rec->data[0] << 8);
    }
    if(at >= o.off32 && at < o.off32 + rec->length) {
      if(roffs)
        *roffs = o.off32;
      return rec;
    }
  }
  return 0;
}