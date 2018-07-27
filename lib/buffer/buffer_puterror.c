#include "../buffer.h"
#include <errno.h>
#include <string.h>

int
buffer_puterror(buffer* b) {
  return buffer_puts(b, strerror(errno));
}
