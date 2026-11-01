#include "../byte.h"
#include <string.h>

/* byte_fill sets the bytes out[0], out[1], ..., out[len - 1] to c */
void
byte_fill(void* out, size_t len, int c) {
  memset(out, c, len);
}
