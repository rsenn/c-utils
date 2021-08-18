#include "../ihex.h"

int
ihex_record_address(ihex_record* rec, ihex_addr* addr) {
  if(rec->type == 4) {
    uint16_unpack_big(rec->data, &addr->hi16);
    return 0;
  }

  addr->lo16 = rec->offset;
  return 1;
}
