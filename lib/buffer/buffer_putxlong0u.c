#include "../buffer.h"
#include "../fmt.h"

int
buffer_putxlong0u(buffer* b, unsigned long l, int pad) {
  char x[FMT_XLONG];
  size_t n = fmt_xlongu(x, l);

  if(buffer_putnc(b, '0', pad - n) < 0)
    return -1;

  return buffer_put(b, x, n);
}
