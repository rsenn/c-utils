#include "../io_internal.h"

int
io_timedout(fd_t d) {
  tai6464 now;
  io_entry* e = (io_entry*)iarray_get((iarray*)io_getfds(), d);
  taia_now(&now);
  return e && e->timeout.sec.x && taia_less(&e->timeout, &now);
}
