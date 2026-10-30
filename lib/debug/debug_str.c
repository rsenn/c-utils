#include "../debug.h"

void
debug_str(const char* name, const char* s) {
  buffer_puts(debug_buf, name);
  buffer_puts(debug_buf, s ? ": '" : ": ");
  buffer_puts(debug_buf, s ? s : "NULL");
  buffer_putsflush(debug_buf, s ? "'\n" : "\n");
}
