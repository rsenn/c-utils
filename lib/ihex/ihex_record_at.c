#include "../ihex.h"
#include "../slist.h"
#include "../uint32.h"

ihex_record*
ihex_record_at(ihex_file* ihf, uint32 at, uint32* roffs) {
  union {
    ihex_record* rec;
    void* ptr;
  } it;
  union {
    uint32 off32;
    struct {
      uint16 lo16;
      uint16 hi16;
    };
  } o;

  o.off32 = 0;

  slink_foreach(ihf->records, it.ptr) {
    if(it.rec->type == 0) {
      o.lo16 = it.rec->offset;
    } else if(it.rec->type == 4) {
      o.hi16 = it.rec->data[1] | (it.rec->data[0] << 8);
    }
    if(at >= o.off32 && at < o.off32 + it.rec->length) {
      if(roffs)
        *roffs = o.off32;
      return it.rec;
    }
  }
  return 0;
}
