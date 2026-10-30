#include "../ihex.h"
#include "../byte.h"

/* ihex_write_eof writes the end of file record :00000001FF */
void
ihex_write_eof(buffer* b) {
  ihex_record r;

  byte_zero(&r, sizeof(r));
  r.type = IHEX_EOF;
  r.checksum = ihex_record_checksum(&r);
  ihex_record_write(&r, b);
}
