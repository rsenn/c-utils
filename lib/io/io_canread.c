#include "../windoze.h"

#if WINDOWS_NATIVE
#else
#endif

#include "../io_internal.h"
#include <errno.h>

void io_wantread_really(fd_t d, io_entry* e);

int64
io_canread() {
  io_entry* e;
  if(first_readable == -1)
#if defined(HAVE_SIGIO)
  {
    if(alt_firstread >= 0 && (e = iarray_get(io_getfds(), alt_firstread)) && e->canread) {
      debug_printf(
          ("io_canread: normal read queue is empty, swapping in alt read queue (starting with %ld)\n", alt_firstread));
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
    e = iarray_get(io_getfds(), first_readable);
    if(!e)
      break;
    r = first_readable;
    first_readable = e->next_read;
    e->next_read = -1;
    debug_printf(("io_canread: dequeue %lld from normal read queue (next is %ld)\n", r, first_readable));

    if(e->wantread &&
#if WINDOWS_NATIVE
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
      return r;
    }
  }
  return -1;
}
