#include "../ihex.h"
#include "../buffer.h"

void
ihex_record_dump(ihex_record* ihr, uint16 hi16, buffer* b) {
  size_t i;
  ihex_record* extlinaddr;
  ihex_addr a;
  a.off32 = ihr->offset;

  buffer_putspad(b, ihex_typestr(ihr->type), 10);
  buffer_puts(b, " 0x");

  a.hi16 = hi16;

  buffer_putxlong0u(b, a.off32, 8);
  buffer_puts(b, "  |");

  /*  buffer_puts(b, " [");
    buffer_putulong0(b, ihr->length, 2);
    buffer_puts(b, "]");*/

  for(i = 0; i < ihr->length; i++) {
    buffer_putspace(b);
    buffer_putxlong0u(b, ihr->data[i], 2);
  }
  buffer_puts(b, " |");
  buffer_puts(b, "  CHECKSUM ");
  buffer_putxlong0u(b, ihr->checksum, 2);
  buffer_puts(b, "");
  buffer_putnlflush(b);
}
