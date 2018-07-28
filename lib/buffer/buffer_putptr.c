#include "../buffer.h"
#include "../fmt.h"

static const size_t ptr_size_2 = sizeof(void*) * 2;

int
buffer_putptr(buffer* b, void* ptr) {
  if(ptr == NULL)
    return buffer_puts(b, "(null)");

  char buf[FMT_XLONG + 1];
  size_t n = fmt_xlonglong(buf, (unsigned long long)ptr);
  buf[n] = '\0';
  buffer_put(b, "0x", 2);
  while(n++ < ptr_size_2)
    buffer_putc(b, '0');
  buffer_puts(b, buf);
  return n + 2;
}
