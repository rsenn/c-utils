#include "../ihex.h"
#include "../uint16.h"

uint8
ihex_record_checksum(const ihex_record* ihr) {
  uint8 ret, offs[2];

  uint16_pack_big((void*)offs, ihr->offset);

  ret = ihr->length + offs[0] + offs[1] + ihr->type;

  for(int i = 0; i < ihr->length; i++)
    ret += ihr->data[i];

  return (~ret) + 1;
}
