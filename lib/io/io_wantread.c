//#define _WINSOCKAPI_
#define _CYGWIN_IF_H_
#include "../windoze.h"

#if WINDOWS_NATIVE
#include <winsock.h>
//#define USE_WS2_32 1
#define __INSIDE_CYGWIN_NET__ 1
#endif
#include "../socket_internal.h"
#include "../io_internal.h"

#include <errno.h>
#include <fcntl.h>

#ifdef HAVE_SIGIO
#include <poll.h>
#endif

#ifdef DEBUG
#include <assert.h>
#else
#define assert(x)
#endif

#ifndef AF_INET
#define AF_INET 2
#endif

#ifndef SOCK_STREAM
#define SOCK_STREAM 1
#endif

void
io_wantread_really(fd_t d, io_entry* e) {
  int64 newfd;
  assert(!e->kernelwantread);
  newfd = !e->kernelwantwrite;
  io_wanted_fds += newfd;
#ifdef HAVE_EPOLL
  if(io_waitmode == EPOLL) {
    struct epoll_event x;
    byte_zero(&x, sizeof(x)); /* to shut up valgrind */
    x.events = EPOLLIN;
    if(e->kernelwantwrite)
      x.events |= EPOLLOUT;
    x.data.fd = d;
    epoll_ctl(io_master, e->kernelwantwrite ? EPOLL_CTL_MOD : EPOLL_CTL_ADD, d, &x);
  }
#endif

#ifdef HAVE_KQUEUE
  if(io_waitmode == KQUEUE) {
    struct kevent kev;
    struct timespec ts;
    EV_SET(&kev, d, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, 0);
    ts.tv_sec = 0;
    ts.tv_nsec = 0;
    kevent(io_master, &kev, 1, 0, 0, &ts);
  }
#endif

#ifdef HAVE_SIGIO
  if(io_waitmode == _SIGIO) {
    struct pollfd p;
    if(e->canread == 0) {
      p.fd = d;
      p.events = POLLIN;
      switch(poll(&p, 1, 0)) {
        case 1: e->canread = 1; break;
        case -1: return;
      }
    }
    if(e->canread) {
      debug_printf(("io_wantread: enqueueing %lld in normal read queue (next is %ld)\n", d, first_readable));
      e->next_read = first_readable;
      first_readable = d;
    }
  }
#endif

#if WINDOWS_NATIVE
  if(e->listened) {
    if(e->next_accept == 0)
      e->next_accept = socket(AF_INET, SOCK_STREAM, 0);
    if(e->next_accept != -1) {
      AcceptEx(d, e->next_accept, e->inbuf, 0, 200, 200, &e->errorcode, &e->or);
      e->acceptqueued = 1;
    }
  } else if(!e->wantread) {
    if(ReadFile((HANDLE)(size_t)d, e->inbuf, sizeof(e->inbuf), &e->errorcode, &e->or)) {
    queueread:
      /* had something to read immediately.  Damn! */
      e->readqueued = 0;
      e->canread = 1;
      e->bytes_read = e->errorcode;
      e->errorcode = 0;
      e->next_read = first_readable;
      first_readable = d;
      return;
    } else if(GetLastError() == ERROR_IO_PENDING)
      e->readqueued = 1;
    else
      goto queueread;
#if 0
    e->next_read = first_readable;
    first_readable = d;
#endif
  }
#endif
  e->wantread = 1;
  e->kernelwantread = 1;
}

void
io_wantread(fd_t d) {
  io_entry* e = iarray_get(io_getfds(), d);
  if(!e || e->wantread)
    return;
  if(e->canread) {
    e->next_read = first_readable;
    first_readable = d;
    e->wantread = 1;
    return;
  }
  /* the harder case: do as before */
  if(!e->kernelwantread)
    io_wantread_really(d, e);
  else
    e->wantread = 1;
}
