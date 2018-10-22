#include "../windoze.h"

#if WINDOWS_NATIVE
#include <io.h>
#else
#include <unistd.h>
#endif

#include "../buffer.h"

void
buffer_close(buffer* b) {
  if(b->deinit) b->deinit(b);
  if(b->fd > 2) close(b->fd);
}
