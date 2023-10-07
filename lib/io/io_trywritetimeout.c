#include "../io_internal.h"
#include "../socket_internal.h"
#include <errno.h>

#if WINDOWS_NATIVE
#ifndef ETIMEDOUT
#define ETIMEDOUT WSAETIMEDOUT
#endif
#endif

int64
io_trywritetimeout(fd_t d, const char* buf, int64 len) {
  int64 r = io_trywrite(d, buf, len);
  if(r == -1) {
    tai6464 x;
    io_entry* e = (io_entry*)iarray_get((iarray*)io_getfds(), d);
    taia_now(&x);
    if(!taia_less(&x, &e->timeout)) {
      errno = ETIMEDOUT;
      r = -2;
    }
  }
  return r;
}
