#include "../buffer.h"
#include "../fmt.h"

int
buffer_putxint64(buffer* b, uint64 l) {
  char buf[FMT_XLONG];
  return buffer_put(b, buf, fmt_xint64(buf, l));
}
