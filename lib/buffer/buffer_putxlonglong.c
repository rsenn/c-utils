#include "../buffer.h"
#include "../fmt.h"

int
buffer_putxlonglong(buffer* b, uint64 l) {
  char x[FMT_XLONG];

  return buffer_put(b, x, fmt_xlonglong(x, l));
}
