#include "../windoze.h"

#if WINDOWS_NATIVE
#include <io.h>
#ifndef __LCC__
#define close _close
#endif
#else
#include <unistd.h>
#endif

#include "../buffer.h"

typedef void deinit_fn_t(buffer*);
void
buffer_close(buffer* b) {
  /* deinit may free the buffer itself (xml_scanner does) */
  fd_type fd = b->fd;

  if(b->deinit)
    ((deinit_fn_t*)b->deinit)(b);

  if(fd > 2)
    close(fd);
}
