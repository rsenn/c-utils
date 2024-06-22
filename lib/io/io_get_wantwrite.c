#include "../io_internal.h"

int
io_get_wantwrite(fd_type d) {
  io_entry* e = (io_entry*)iarray_get((iarray*)io_getfds(), d);
  return e->wantwrite || e->kernelwantwrite;
}
