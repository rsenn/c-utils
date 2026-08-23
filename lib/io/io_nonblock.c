#include "../io_internal.h"
#include "../windoze.h"
#include "../socket_internal.h"
#include "../ndelay.h"

#if WINDOWS_NATIVE
#undef __BSD_VISIBLE
#define __BSD_VISIBLE 0
#define _WINSOCKAPI_
#ifndef FIONBIO
#define FIONBIO 0x8004667e /* set/clear non-blocking i/o */
#endif

#endif

#include <errno.h>

void
io_nonblock(fd_type d) {
  io_entry* e = (io_entry*)iarray_get((iarray*)io_getfds(), d);

#ifdef DEBUG_IO
  buffer_putspad(buffer_2, "io_nonblock", 30);
  buffer_puts(buffer_2, " d=");
  buffer_putlonglong(buffer_2, d);
  buffer_putnlflush(buffer_2);
#endif
#if WINDOWS_NATIVE
  unsigned long i = 1;

  if(ioctlsocket(d, FIONBIO, &i) == 0)

    if(e)
      e->nonblock = 1;
#else
  if(ndelay_on(d) == 0)
    if(e)
      e->nonblock = 1;
#endif
}
