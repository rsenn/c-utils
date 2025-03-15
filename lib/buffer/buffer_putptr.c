#include "../buffer.h"
#include "../fmt.h"

const size_t buffer_putptr_size_2 = sizeof(void*) * 2;

int
buffer_putptr(buffer* b, void* ptr) {
  char x[FMT_XLONG + 1];
  size_t n;

  if(ptr == NULL)
    return buffer_puts(b, "(null)");

  n = fmt_xlonglong(x, (uint64)(intptr_t)ptr);
  x[n] = '\0';

  while(n++ < buffer_putptr_size_2)
    buffer_putc(b, '0');

  buffer_puts(b, x);
  return n + 2;
}
