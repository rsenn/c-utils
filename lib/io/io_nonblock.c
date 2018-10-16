#include "../socket.h"

#if WINDOWS_NATIVE
#undef __BSD_VISIBLE
#define __BSD_VISIBLE 0
#define _WINSOCKAPI_
#ifndef FIONBIO
#define FIONBIO     0x8004667e		 /* set/clear non-blocking i/o */
#endif
#endif
#include <fcntl.h>
#include <errno.h>
#include "../io_internal.h"

#ifndef O_NDELAY
#define O_NDELAY O_NONBLOCK
#endif

void io_nonblock(fd_t d ){
  io_entry* e = iarray_get(io_getfds(), d);
#if WINDOWS_NATIVE
  unsigned long i = 1;
  if(ioctlsocket(d, FIONBIO, &i) == 0)
    if(e) e->nonblock = 1;
#else
  if(fcntl(d, F_SETFL, fcntl(d, F_GETFL, 0) | O_NDELAY) == 0)
    if(e) e->nonblock = 1;
#endif
}
