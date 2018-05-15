#include <buffer.h>
#ifndef _WIN32
#include <unistd.h>
#else
#include <io.h>
#endif

void
buffer_close(buffer* b) {
  if(b->fd > 2) close(b->fd);
  if(b->deinit) b->deinit(b);
}
