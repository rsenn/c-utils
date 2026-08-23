#include "../io_internal.h"
#include "../windoze.h"
#include "../socket_internal.h"
#include "../ndelay.h"

#include <errno.h>

void
io_block(fd_type d) {
  io_entry* e = (io_entry*)iarray_get((iarray*)io_getfds(), d);
#if WINDOWS_NATIVE
  unsigned long i = 0;

  if(ioctlsocket(d, FIONBIO, &i) == 0)

    if(e)
      e->nonblock = 0;
#else
  if(ndelay_off(d) == 0)
    if(e)
      e->nonblock = 0;
#endif
}
