#include "../buffer.h"
#include "../tai.h"
#include "../fmt.h"

int
buffer_puttai(buffer* b, const tai64* t) {
  char x[FMT_ISO8601];

  return buffer_put(b, x, fmt_tai(x, t));
}
