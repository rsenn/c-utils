#include "../buffer.h"
#include "../fmt.h"

int
buffer_putxlonglong(buffer* b, unsigned long long l) {
  char buf[FMT_XLONG];
  return buffer_put(b, buf, fmt_xlonglong(buf, l));
}
