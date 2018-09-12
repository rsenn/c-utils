#include "../windoze.h"
#if WINDOWS
#include "../io_internal.h"
#else
#include <unistd.h>
#endif
#include "../charbuf.h"

void
charbuf_close(charbuf* b) {
  if(b->fd > 2) close(b->fd);
}
