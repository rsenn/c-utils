#include "../windoze.h"

#if WINDOWS
#else
#endif

#include "../io_internal.h"

void*
io_getcookie(fd_t d) {
  io_entry* e;
  e = iarray_get(io_getfds(), d);
  return e ? e->cookie : 0;
}
