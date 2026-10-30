#include "../debug.h"

void
debug_sl(const char* name, const strlist* l, const char* sep) {
  size_t pos, n;
  const char* x;
  buffer_puts(debug_buf, name);
  buffer_puts(debug_buf, ":\n  ");
  strlist_foreach(l, x, n) {
    if(n)
      buffer_puts(debug_buf, sep);
    buffer_puts(debug_buf, "#");
    buffer_putulong(debug_buf, n++);
    buffer_puts(debug_buf, ": ");
    buffer_put(debug_buf, x, n);
  }
  buffer_putnlflush(debug_buf);
}
