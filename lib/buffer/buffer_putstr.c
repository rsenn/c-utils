#include "../buffer.h"

void
buffer_putstr(buffer* b, const char* x) {
  if(x) {
    buffer_putc(b, '"');
    buffer_puts(b, x);
    buffer_putc(b, '"');
  } else {
    buffer_puts(b, "NULL");
  }
}
