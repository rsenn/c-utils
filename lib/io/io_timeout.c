#include "../io_internal.h"

void
io_timeout(fd_t d, tai6464 t) {
  io_entry* e = (io_entry*)iarray_get((iarray*)io_getfds(), d);
  if(e)
    e->timeout = t;
}
