#include "../windoze.h"

#if WINDOWS_NATIVE
#include <io.h>
#else
#include <unistd.h>
#endif

#include "../charbuf.h"

void
charbuf_close(charbuf* b) {
  if(b->fd > 2) close(b->fd);
}
