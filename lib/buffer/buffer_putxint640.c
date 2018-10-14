#include "../buffer.h"
#include "../fmt.h"
#include <stdlib.h>

int
buffer_putxint640(buffer* b, uint64 l, int pad) {
  char buf[FMT_XLONG];
  size_t n = fmt_xint64(buf, l);

  if(buffer_putnc(b, '0', pad - n) < 0) return -1;

  return buffer_put(b, buf, n);
}
