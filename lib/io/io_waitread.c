#include "../io_internal.h"
#include "../windoze.h"
#include "../uint64.h"
#if WINDOWS_NATIVE
#include <winsock.h>
#include <windows.h>
#include <io.h>
#else
#include <poll.h>
#endif

#include "../socket_internal.h"

#include <errno.h>

#if WINDOWS_NATIVE

int64
io_waitread(fd_type d, char* buf, int64 len) {
  long r;
  io_entry* e = (io_entry*)iarray_get((iarray*)io_getfds(), d);

  if(!e) {
    errno = EBADF;
    return -3;
  }

  if(e->nonblock) {
    unsigned long i = 0;
    ioctlsocket(d, FIONBIO, &i);
  }
  r = read(d, buf, len);

  if(e->nonblock) {
    unsigned long i = 1;
    ioctlsocket(d, FIONBIO, &i);
  }

  if(r == -1)
    r = -3;

  return r;
}

#else

int64
io_waitread(fd_type d, char* buf, int64 len) {
  ssize_t r;
  struct pollfd p;
  io_entry* e = (io_entry*)iarray_get((iarray*)io_getfds(), d);

  if(!e) {
    errno = EBADF;
    return -3;
  }

  if(e->nonblock) {
  again:
    p.fd = d;

    if(p.fd != d) {
      errno = EBADF;
      return -3;
    } /* catch overflow */

    p.events = POLLIN;

    switch(poll(&p, 1, -1)) {
      case -1:
        if(errno == EAGAIN)
          goto again;

        return -3;
    }
  }

  r = read(d, buf, len);

  if(r == -1) {
    if(errno == EAGAIN)
      goto again;

    r = -3;
  }

  return r;
}

#endif
