#include "../ihex.h"

int
ihex_record_address(ihex_record* rec, ihex_addr* addr) {
  int type = rec->type;

  if(type == 4) {
    uint16_unpack_big(rec->data, &addr->hi16);
    addr->lo16 = 0;
  } else {
    addr->lo16 = rec->offset;
  }
  return type;
}
