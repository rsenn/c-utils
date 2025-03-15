#include "../buffer.h"
#include "../fmt.h"

int
buffer_putulonglong(buffer* b, uint64 i) {
  char x[FMT_ULONG];

  return buffer_put(b, x, fmt_ulonglong(x, i));
}
