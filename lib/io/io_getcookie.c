#include "../io_internal.h"
#include "../windoze.h"

#if WINDOWS_NATIVE
#else
#endif

void*
io_getcookie(fd_t d) {
  io_entry* e;
  e = (io_entry*)iarray_get((iarray*)io_getfds(), d);
  return e ? e->cookie : 0;
}
