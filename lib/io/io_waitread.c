#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#else
#include <poll.h>
#endif
#include "../io_internal.h"
#include <errno.h>

#if defined(_WIN32) || defined(_WIN64)

int64
io_waitread(fd_t d, char* buf, int64 len) {
  long r;
  io_entry* e = iarray_get(io_getfds(), d);
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
  if(r == -1) r = -3;
  return r;
}

#else

int64
io_waitread(fd_t d, char* buf, int64 len) {
  long r;
  struct pollfd p;
  io_entry* e = iarray_get(io_getfds(), d);
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
        if(errno == EAGAIN) goto again;
        return -3;
    }
  }
  r = read(d, buf, len);
  if(r == -1) {
    if(errno == EAGAIN) goto again;
    r = -3;
  }
  return r;
}

#endif
