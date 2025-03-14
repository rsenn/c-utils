#include "../io_internal.h"
#include "../windoze.h"

#include <errno.h>

void io_wantwrite_really(fd_type d, io_entry* e);

int64
io_canwrite() {
  ssize_t ret = -1;
  io_entry* e;

  if(first_writeable == -1)
#if defined(HAVE_SIGIO)
  {
    if(alt_firstwrite >= 0 && (e = (io_entry*)iarray_get((iarray*)io_getfds(), alt_firstwrite)) && e->canwrite) {
      debug_printf(("io_canwrite: normal write queue is empty, swapping in alt                     rite queue                     starting with %ld)\n", alt_firstwrite));
      first_writeable = alt_firstwrite;
      alt_firstwrite = -1;
    } else
      return -1;
  }
#else
    return -1;
#endif

  for(;;) {
    int64 r;

    if(!(e = (io_entry*)iarray_get((iarray*)io_getfds(), first_writeable)))
      break;

    r = first_writeable;
    first_writeable = e->next_write;
    e->next_write = -1;
    debug_printf(("io_canwrite: dequeue %lld from normal write queue                   next is %ld)\n", r, first_writeable));

    if(e->wantwrite &&
#if WINDOWS_NATIVE && !defined(USE_SELECT)
       (e->canwrite || e->sendfilequeued == 1)
#else
       e->canwrite
#endif
    ) {
#if defined(HAVE_SIGIO)
      e->next_write = alt_firstwrite;
      alt_firstwrite = r;
      debug_printf(("io_canwrite: enqueue %ld in alt write queue (next is %ld)\n", alt_firstwrite, e->next_write));

      if(io_waitmode != _SIGIO)
#endif
        e->canwrite = 0;

      if(!e->kernelwantwrite)
        io_wantwrite_really(r, e);

      ret = r;
      break;
    }
  }

#ifdef DEBUG_IO
  if(ret != -1) {
    buffer_putspad(buffer_2, "io_canwrite ", 30);
    buffer_puts(buffer_2, "ret=");
    buffer_putlong(buffer_2, ret);
    buffer_putnlflush(buffer_2);
  }
#endif
  return ret;
}
