#include "../buffer.h"
#include "../fmt.h"

int
buffer_putlonglong(buffer* b, int64 i) {
  char x[FMT_LONG];

  return buffer_put(b, x, fmt_longlong(x, i));
}
