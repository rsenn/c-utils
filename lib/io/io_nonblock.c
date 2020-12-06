#include "../io.h"
#include "../windoze.h"
#include "../socket_internal.h"

#if WINDOWS_NATIVE
#undef __BSD_VISIBLE
#define __BSD_VISIBLE 0
#define _WINSOCKAPI_
#ifndef FIONBIO
#define FIONBIO 0x8004667e /* set/clear non-blocking i/o */
#endif

#endif

#include <fcntl.h>
#include <errno.h>
#include "../io_internal.h"

#ifndef O_NONBLOCK
#define O_NONBLOCK O_NDELAY
#endif

void
io_nonblock(fd_t d) {
  io_entry* e;

#ifdef DEBUG_IO
  buffer_putspad(buffer_2, "io_nonblock", 30);
  buffer_puts(buffer_2, " d=");
  buffer_putlonglong(buffer_2, d);
  buffer_putnlflush(buffer_2);
#endif
  e = (io_entry*)iarray_get((iarray*)io_getfds(), d);
#if WINDOWS_NATIVE
  unsigned long i = 1;
  if(ioctlsocket(d, FIONBIO, &i) == 0)
    if(e)
      e->nonblock = 1;
#else
  if(fcntl(d, F_SETFL, fcntl(d, F_GETFL, 0) | O_NONBLOCK) == 0)
    if(e)
      e->nonblock = 1;
#endif
}
