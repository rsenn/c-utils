#include "../io_internal.h"

void
io_eagain_read(fd_type d) {
  io_entry* e = (io_entry*)iarray_get((iarray*)io_getfds(), d);

  if(e) {
    e->canread = 0;
#if defined(HAVE_SIGIO)
    if(d == alt_firstread) {
      debug_printf(("io_eagain: dequeueing %lld from alt read queue (next is %ld)\n", d, e->next_read));
      alt_firstread = e->next_read;
      e->next_read = -1;
    }
#endif
  }
}
