#include "../socket.h"

#include "../io_internal.h"

#if WINDOWS_NATIVE
#else
#include <fcntl.h>
#endif

#include <errno.h>

#ifndef O_NDELAY
#define O_NDELAY O_NONBLOCK
#endif

void io_block(fd_t d ){
  io_entry* e = iarray_get(io_getfds(), d);
#if WINDOWS_NATIVE
  unsigned long i = 0;
  if(ioctlsocket(d, FIONBIO, &i) == 0)
    if(e) e->nonblock = 0;
#else
  if(fcntl(d, F_SETFL, fcntl(d, F_GETFL, 0) & ~O_NDELAY) == 0)
    if(e) e->nonblock = 0;
#endif
}
