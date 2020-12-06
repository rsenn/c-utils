#include "../io.h"
#include "../socket_internal.h"

#if WINDOWS_NATIVE
#else
#endif

#include "../io_internal.h"
#include <errno.h>
#include <fcntl.h>

#ifdef DEBUG
#include <assert.h>
#else
#define assert(x)
#endif

/* IDEA: if someone calls io_dontwantwrite, do not do the syscall to
 * tell the kernel about it.  Only when a write event comes in and the
 * user has told us he does not want them, THEN tell the kernel we are
 * not interested.  In the typical protocol case of "write request, read
 * reply", this should save a lot of syscalls. */

void
io_dontwantwrite_really(fd_t d, io_entry* e) {
  int64 newfd;
  (void)d;
  assert(e->kernelwantwrite);
  newfd = !e->kernelwantread;
  io_wanted_fds -= newfd;
#ifdef HAVE_EPOLL
  if(io_waitmode == EPOLL) {
    struct epoll_event x;
    byte_zero(&x, sizeof(x)); /* to shut up valgrind */
    x.events = 0;
    if(e->wantread)
      x.events |= EPOLLIN;
    x.data.fd = d;
    epoll_ctl(io_master, e->kernelwantread ? EPOLL_CTL_MOD : EPOLL_CTL_DEL, d, &x);
  }
#endif

#ifdef HAVE_KQUEUE
  if(io_waitmode == KQUEUE) {
    struct kevent kev;
    struct timespec ts;
    EV_SET(&kev, d, EVFILT_WRITE, EV_DELETE, 0, 0, 0);
    ts.tv_sec = 0;
    ts.tv_nsec = 0;
    kevent(io_master, &kev, 1, 0, 0, &ts);
  }
#endif

  e->wantwrite = 0;
  e->kernelwantwrite = 0;
}

void
io_dontwantwrite(fd_t d) {
  io_entry* e;
#ifdef DEBUG_IO
  buffer_putspad(buffer_2, "io_dontwantwrite", 30);
  buffer_puts(buffer_2, "d=");
  buffer_putlonglong(buffer_2, d);
  buffer_putnlflush(buffer_2);
#endif
  e = (io_entry*)iarray_get((iarray*)io_getfds(), d);
  if(e) {
    if(e->canwrite)
      io_dontwantwrite_really(d, e);
    e->wantwrite = 0;
  }
}
