#include "../io_internal.h"
#include "../windoze.h"
#include "../socket_internal.h"

#if WINDOWS_NATIVE
#else
#include <fcntl.h>
#endif

#include <errno.h>

#ifndef O_NONBLOCK
#define O_NONBLOCK O_NDELAY
#endif

void
io_block(fd_t d) {
  io_entry* e = (io_entry*)iarray_get((iarray*)io_getfds(), d);
#if WINDOWS_NATIVE
  unsigned long i = 0;
  if(ioctlsocket(d, FIONBIO, &i) == 0)
    if(e)
      e->nonblock = 0;
#else
  if(fcntl(d, F_SETFL, fcntl(d, F_GETFL, 0) & ~O_NONBLOCK) == 0)
    if(e)
      e->nonblock = 0;
#endif
}
