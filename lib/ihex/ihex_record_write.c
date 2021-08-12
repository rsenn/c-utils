#include "../ihex.h"
#include "../buffer.h"

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

void
ihex_record_write(ihex_record* ihr, buffer* b) {
  size_t i;
  buffer_putc(b, ':');
  buffer_putxlong0u(b, ihr->length, 2);
  buffer_putxlong0u(b, ihr->offset, 4);
  buffer_putxlong0u(b, ihr->type, 2);

  for(i = 0; i < ihr->length; i++)
    buffer_putxlong0u(b, ihr->data[i], 2);

  buffer_putxlong0u(b, ihr->checksum, 2);
  buffer_putnlflush(b);
}
