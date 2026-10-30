#include "../debug.h"

void
debug_int(const char* name, int i) {
  buffer_puts(debug_buf, name);
  buffer_puts(debug_buf, ": ");
  buffer_putlong(debug_buf, i);
  buffer_putnlflush(debug_buf);
}
