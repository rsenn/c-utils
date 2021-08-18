#include "../ihex.h"
#include "../buffer.h"

void
ihex_record_dump(ihex_record* ihr, buffer* b) {
  size_t i;
  buffer_puts(b, "ihex_record { offset: ");
  buffer_putxlong0u(b, ihr->offset, 4);
  buffer_puts(b, ", checksum: ");
  buffer_putxlong0u(b, ihr->checksum, 2);
  buffer_puts(b, ", type: ");
  buffer_putxlong0u(b, ihr->type, 2);
  buffer_puts(b, ", data(");
  buffer_putxlong0u(b, ihr->length, 2);
  buffer_puts(b, "):");
 
  for(i = 0; i < ihr->length; i++) {
    buffer_putspace(b);
    buffer_putxlong0u(b, ihr->data[i], 2);
  }

  buffer_putnlflush(b);
}
