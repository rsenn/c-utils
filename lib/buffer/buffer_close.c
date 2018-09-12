#include "../windoze.h"
#if WINDOWS
#include "../io_internal.h"
#else
#endif
#include "../buffer.h"

void
buffer_close(buffer* b) {
  if(b->fd > 2) close(b->fd);
  if(b->deinit) b->deinit(b);
}
