#include "../ihex.h"
#include "../uint16.h"

uint8
ihex_record_checksum(const ihex_record* ihr) {
  uint8 offs[2];
  uint8 ret;
  int i;

  uint16_pack_big(offs, ihr->offset);

  ret = ihr->length + offs[0] + offs[1] + ihr->type;

  for(i = 0; i < ihr->length; i++)
    ret += ihr->data[i];

  return (~ret) + 1;
}
