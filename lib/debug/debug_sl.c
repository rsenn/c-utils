#include "../debug.h"

/* "name:" and then "#index: element" for every element of the list (the element
 * length used to double as the counter, which printed one garbled entry) */
void
debug_sl(const char* name, const strlist* l, const char* sep) {
  size_t n, i = 0;
  const char* x;

  buffer_puts(debug_buf, name);
  buffer_puts(debug_buf, ":\n  ");
  strlist_foreach(l, x, n) {
    if(i)
      buffer_puts(debug_buf, sep);
    buffer_puts(debug_buf, "#");
    buffer_putulong(debug_buf, i++);
    buffer_puts(debug_buf, ": ");
    buffer_put(debug_buf, x, n);
  }
  buffer_putnlflush(debug_buf);
}
