#include "../buffer.h"
#include "../fmt.h"

int
buffer_putxlong(buffer* b, unsigned long l) {
  char x[FMT_XLONG];

  return buffer_put(b, x, fmt_xlong(x, l));
}
