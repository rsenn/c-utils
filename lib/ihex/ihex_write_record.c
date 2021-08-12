#include "../ihex.h"
#include "../buffer.h"

void
ihex_write_record(ihex_record* ihr, buffer* b) {
  size_t i;
  buffer_putxlong0(b, ihr->length, 2);
  buffer_putxlong0(b, ihr->offset, 4);
  buffer_putxlong0(b, ihr->type, 2);

  for(i = 0; i < ihr->length; i++)
    buffer_putxlong0(b, ihr->data[i], 2);

  buffer_putxlong0(b, ihr->checksum, 2);
  buffer_putnlflush(b);
}
