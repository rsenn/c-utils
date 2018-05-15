#include "buffer.h"
#include "fmt.h"

int
buffer_putuint64(buffer* b, uint64 i) {
  char buf[FMT_ULONG];
  return buffer_put(b, buf, fmt_uint64(buf, i));
}
