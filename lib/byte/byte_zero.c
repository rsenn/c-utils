#include "../byte.h"
#include <string.h>

/* byte_zero sets the bytes out[0], out[1], ..., out[len - 1] to 0 */
void
byte_zero(void* out, size_t len) {
  memset(out, 0, len);
}
