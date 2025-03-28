#include "../io_internal.h"

void
io_eagain_write(fd_type d) {
  io_entry* e = (io_entry*)iarray_get((iarray*)io_getfds(), d);

  if(e) {
    e->canwrite = 0;
#if defined(HAVE_SIGIO)
    if(d == alt_firstwrite) {
      debug_printf(("io_eagain: dequeueing %lld from alt write queue                     next is %ld)\n", d, e->next_write));
      alt_firstwrite = e->next_write;
      e->next_write = -1;
    }
#endif
  }
}
