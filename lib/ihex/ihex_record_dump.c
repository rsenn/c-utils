#include "../ihex.h"
#include "../buffer.h"

void
ihex_record_dump(ihex_record* ihr, buffer* b) {
  size_t i;
  buffer_putspad(b, ihex_typestr(ihr->type), 10);
  buffer_puts(b, " 0x");
  buffer_putxlong0u(b, ihr->offset, 8);
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
