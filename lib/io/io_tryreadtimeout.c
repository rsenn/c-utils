#include "../io_internal.h"
#include "../windoze.h"
#include <errno.h>

#if WINDOWS && !defined(ETIMEDOUT)
#define ETIMEDOUT WSAETIMEDOUT 
#endif

int64
io_tryreadtimeout(fd_t d, char* buf, int64 len) {
  int64 r = io_tryread(d, buf, len);
  if(r == -1) {
    tai6464 x;
    io_entry* e = iarray_get(io_getfds(), d);
    taia_now(&x);
    if(!taia_less(&x, &e->timeout)) {
      errno = ETIMEDOUT;
      r = -2;
    }
  }
  return r;
}
