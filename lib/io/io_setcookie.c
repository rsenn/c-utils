#include "../io.h"
#include "../windoze.h"

#if WINDOWS_NATIVE
#else
#endif

#include "../io_internal.h"

void
io_setcookie(fd_t d, void* cookie) {
  io_entry* e;
  if((e = (io_entry*)iarray_get((iarray*)io_getfds(), d)))
    e->cookie = cookie;
}
