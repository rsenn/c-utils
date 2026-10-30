#include "../debug.h"

void
debug_byte(const char* name, const char* x, size_t n) {
  buffer_puts(debug_buf, name);
  buffer_puts(debug_buf, ": ");
  buffer_put(debug_buf, x, n);
  buffer_putsflush(debug_buf, debug_nl);
}
