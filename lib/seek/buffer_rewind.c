#include "../buffer.h"
#include "../seek.h"

void
buffer_rewind(buffer* b) {
  if(b->fd >= 0) {
    seek_set(b->fd, 0);
  }
  b->p = b->n = 0;
}
