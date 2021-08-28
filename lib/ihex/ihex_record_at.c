#include "../ihex.h"
#include "../uint32.h"

ihex_record*
ihex_record_at(ihex_file* ihf, uint32 at, uint32* roffs) {
  ihex_recptr ptr;
  ptr = ihex_record_find(ihf, at, roffs);

  if(ptr.rec && ptr.link != &ihf->records)
    return ptr.rec;
  return 0;
}
