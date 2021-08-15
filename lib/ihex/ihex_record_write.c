#include "../ihex.h"
#include "../buffer.h"

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
