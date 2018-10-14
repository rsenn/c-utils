#include "../buffer.h"
#include "../fmt.h"

int
buffer_putint64(buffer* b, int64 i) {
  char buf[FMT_LONG];
  return buffer_put(b, buf, fmt_int64(buf, i));
}
