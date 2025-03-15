#include "../buffer.h"
#include "../fmt.h"

int
buffer_putlong(buffer* b, signed long l) {
  char x[FMT_LONG];

  return buffer_put(b, x, fmt_long(x, l));
}
