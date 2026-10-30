#include "../debug.h"

void
debug_sa(const char* name, stralloc* sa) {
  buffer_puts(debug_buf, name);
  buffer_puts(debug_buf, ": ");
  buffer_putsa(debug_buf, sa);
  buffer_putnlflush(debug_buf);
}
