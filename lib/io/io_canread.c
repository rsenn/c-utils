#include "../io_internal.h"
#include "../windoze.h"
#ifdef DEBUG_IO
#include "../buffer.h"
#endif
#if WINDOWS_NATIVE
#else
#endif

#include <errno.h>

void io_wantread_really(fd_type d, io_entry* e);

int64
io_canread() {
  ssize_t ret = -1;
  io_entry* e;

  if(first_readable == -1)
#if defined(HAVE_SIGIO)
  {
    if(alt_firstread >= 0 && (e = (io_entry*)iarray_get((iarray*)io_getfds(), alt_firstread)) && e->canread) {
      debug_printf(("io_canread: normal read queue is empty, swapping in alt                     ead queue (starting                     ith %ld)\n", alt_firstread));
      first_readable = alt_firstread;
      alt_firstread = -1;
    } else
      return -1;
  }
#else
    return -1;
#endif

  for(;;) {
    int64 r;

    if(!(e = (io_entry*)iarray_get((iarray*)io_getfds(), first_readable)))
      break;

    r = first_readable;
    first_readable = e->next_read;
    e->next_read = -1;
    debug_printf(("io_canread: dequeue %lld from normal read queue (next is %ld)\n", r, first_readable));

    if(e->wantread &&
#if WINDOWS_NATIVE && !defined(USE_SELECT)
       (e->canread || e->acceptqueued == 1 || e->readqueued == 1)
#else
       e->canread
#endif
    ) {
#if defined(HAVE_SIGIO)
      e->next_read = alt_firstread;
      alt_firstread = r;
      debug_printf(("io_canread: enqueue %ld in alt read queue (next is %ld)\n", alt_firstread, e->next_read));

      if(io_waitmode != _SIGIO)
#endif
        e->canread = 0;

      if(!e->kernelwantread)
        io_wantread_really(r, e);

      ret = r;
      break;
    }
  }

#ifdef DEBUG_IO
  if(ret != -1) {
    buffer_putspad(buffer_2, "\x1b[38;5;165mio_canread\x1b[0m ", 30);
    buffer_puts(buffer_2, "ret=");
    buffer_putlong(buffer_2, ret);
    buffer_putnlflush(buffer_2);
  }
#endif

  return ret;
}
