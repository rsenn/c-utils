#include "../debug.h"

void
debug_strarray(const char* name, const strarray* stra) {
  char** p;
  size_t n = 0;
  buffer_puts(debug_buf, name);
  buffer_puts(debug_buf, ":\n  ");
  strarray_foreach(stra, p) {
    if(n)
      buffer_puts(debug_buf, ", ");
    buffer_puts(debug_buf, "#");
    buffer_putulong(debug_buf, n++);
    buffer_puts(debug_buf, ": ");
    buffer_puts(debug_buf, *p ? *p : "(null)");
  }
  buffer_putnlflush(debug_buf);
}
