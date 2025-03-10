#include "../io_internal.h"
#include "../socket_internal.h"

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <assert.h>

#ifdef HAVE_SIGIO
#include <poll.h>
#endif

/* IDEA: if someone calls io_dontwantwrite, do not do the syscall to
 * tell the kernel about it.  Only when a write event comes in and the
 * user has told us he does not want them, THEN tell the kernel we are
 * not interested.  In the typical protocol case of "write request, read
 * reply", this should save a lot of syscalls.
 * Now, if someone calls io_wantwrite, we might be in the situation that
 * canwrite is already set.  In that case, just enqueue the fd. */

void
io_wantwrite_really(fd_type d, io_entry* e) {
  int64 newfd;

  assert(!e->kernelwantwrite); /* we should not be here if we already told
                                  the kernel we want to write */
  newfd = (!e->kernelwantread);
  io_wanted_fds += newfd;

#ifdef HAVE_EPOLL
  if(io_waitmode == EPOLL) {
    struct epoll_event x;

    byte_zero(&x, sizeof(x)); /* to shut up valgrind */
    x.events = EPOLLOUT;

    if(e->kernelwantread)
      x.events |= EPOLLIN;

    x.data.fd = d;
    epoll_ctl(io_master, e->kernelwantread ? EPOLL_CTL_MOD : EPOLL_CTL_ADD, d, &x);
  }
#endif

#ifdef HAVE_KQUEUE
  if(io_waitmode == KQUEUE) {
    struct kevent kev;
    struct timespec ts;
    EV_SET(&kev, d, EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, 0);
    ts.tv_sec = 0;
    ts.tv_nsec = 0;
    kevent(io_master, &kev, 1, 0, 0, &ts);
  }
#endif

#ifdef HAVE_SIGIO
  if(io_waitmode == _SIGIO) {
    struct pollfd p;
    p.fd = d;
    p.events = POLLOUT;

    switch(poll(&p, 1, 0)) {
      case 1: e->canwrite = 1; break;
      case 0: e->canwrite = 0; break;
      case -1: return;
    }

    if(e->canwrite) {
      debug_printf(("io_wantwrite: enqueueing %lld in normal write queue                     efore %ld\n", d, first_readable));
      e->next_write = first_writeable;
      first_writeable = d;
    }
  }
#endif

#if WINDOWS_NATIVE
  printf("e->wantwrite == %d\n", e->wantwrite);

  if(!e->wantwrite) {
    e->next_write = first_writeable;
    e->canwrite = 1;
    first_writeable = d;
    debug_printf(("queueing write, setting first_writeable to %ld\n", (long)d));
  }
#endif
  e->wantwrite = 1;
  e->kernelwantwrite = 1;
}

void
io_wantwrite(fd_type d) {
  io_entry* e;

#ifdef DEBUG_IO
  buffer_putspad(buffer_2, "io_wantwrite", 30);
  buffer_puts(buffer_2, "d=");
  buffer_putlonglong(buffer_2, d);
  buffer_putnlflush(buffer_2);
#endif

  if(!(e = (io_entry*)iarray_get((iarray*)io_getfds(), d)))
    return;

  if(e->wantwrite && e->kernelwantwrite)
    return;

  if(e->canwrite) {
    e->next_write = first_writeable;
    first_writeable = d;
    e->wantwrite = 1;
    return;
  }

  /* the harder case: do as before */

  if(!e->kernelwantwrite)
    io_wantwrite_really(d, e);
  else
    e->wantwrite = 1;
}
