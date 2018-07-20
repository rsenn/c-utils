#include <stdlib.h>
#include "../textbuf.h"

void
textbuf_free(buffer* b) {
  textbuf* tb = (void*)b->fd;
  free(tb);
}
