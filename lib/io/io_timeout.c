#include "../io_internal.h"

void io_timeout(fd_t d, tai6464 t) {
  io_entry* e = iarray_get(io_getfds(), d);
  if(e)
    e->timeout = t;
}
