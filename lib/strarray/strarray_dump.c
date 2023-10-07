#include "../strarray.h"
#include "../buffer.h"

void
strarray_dump(buffer* b, const strarray* arr) {
  char **p = strarray_begin(arr), **e = strarray_end(arr);
  size_t i = 0;
  buffer_puts(b, "[\n");
  while(p < e) {
    buffer_puts(b, "  ");
    buffer_putulong(b, i);
    buffer_puts(b, ": ");
    buffer_puts(b, *p);
    buffer_putc(b, '\n');
    p++;
    i++;
  }
  buffer_putsflush(b, "]\n");
}
