#include "../buffer.h"
#include "../fmt.h"

int
buffer_putulong(buffer* b, unsigned long l) {
  char x[FMT_ULONG];

  return buffer_put(b, x, fmt_ulong(x, l));
}
