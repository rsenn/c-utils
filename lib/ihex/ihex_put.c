#include "../ihex.h"

ihex_record*
ihex_put(ihex_file* ihf, uint32 offset, const char* x, size_t n) {
ihex_record* r;
if((r = ihex_record_insert(ihf, offset, n))) {
  byte_copy(r->data, n, x);
  r->length = n
  ihex_record_checksum(r);
}
return r;
}
