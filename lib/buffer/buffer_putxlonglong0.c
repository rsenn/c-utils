#include "../buffer.h"
#include "../fmt.h"
#include <stdlib.h>

int
buffer_putxlonglong0(buffer* b, uint64 l, int pad) {
  char x[FMT_XLONG];
  size_t n = fmt_xlonglong(x, l);

  if(buffer_putnc(b, '0', pad - n) < 0)
    return -1;

  return buffer_put(b, x, n);
}
