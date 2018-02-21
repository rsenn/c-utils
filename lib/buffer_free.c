#include <stdlib.h>
#include "buffer.h"

void buffer_free(void* buf) {
  buffer* b=buf;
  free(b->x);
}
