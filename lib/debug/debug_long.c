#include "../debug.h"

void
debug_long(const char* name, long value) {
  buffer_puts(debug_buf, name);
  buffer_puts(debug_buf, ": ");
  buffer_putlong(debug_buf, value);
  buffer_putnlflush(debug_buf);
}
