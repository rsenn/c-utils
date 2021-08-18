#include "../ihex.h"
#include "../uint32.h"

ihex_record*
ihex_record_at(ihex_file* ihf, uint32 at, uint32* roffs) {
  ihex_record** ptr;

  if((ptr = ihex_record_find(ihf, at, roffs)))
    return *ptr;
  return 0;
}
