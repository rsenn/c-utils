#include "../buffer.h"
#include "../fmt.h"

int
buffer_put8long(buffer* b, unsigned long l) {
  char x[FMT_8LONG];

  return buffer_put(b, x, fmt_8long(x, l));
}
