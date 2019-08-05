#ifdef __POCC__
#define NOWINBASEINTERLOCK 1
#define _NTOS_ 1
long __stdcall InterlockedCompareExchange(long volatile*, long, long);
void* __stdcall InterlockedCompareExchangePointer(void* volatile*, void*, void*);
#endif
#include "../windoze.h"
#ifndef _POSIX_SOURCE
#define _POSIX_SOURCE
#endif
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include "../socket_internal.h"
#include "../array.h"

#define my_extern
#include "../io_internal.h"
#undef my_extern
#include "../byte.h"

#if WINDOWS_NATIVE
#include <io.h>
#else
#include <unistd.h>
#endif

#ifdef HAVE_SIGIO
#include <signal.h>
#endif

#include <fcntl.h>
#include <errno.h>

#include "../cas.h"

#ifdef __APPLE__
#define EXPORT __attribute__((visibility("default")))
#elif WINDOWS_NATIVE
#define EXPORT __declspec(dllexport)
#else
#define EXPORT
#endif

#if WINDOWS_NATIVE
#include <stdio.h>
#endif

#include <fcntl.h>

#ifndef F_SETSIG
#define F_SETSIG 10 /* for sockets. */
#define F_GETSIG 11 /* for sockets. */
#endif

static iarray io_fds;

void*
io_getfds() {
  return &io_fds;
}

static
#ifndef __POCC__
    volatile
#endif
    long int io_fds_inited;
uint64 io_wanted_fds;
array io_pollfds;
enum __io_waitmode io_waitmode;
#if defined(HAVE_KQUEUE) || defined(HAVE_EPOLL) || defined(HAVE_DEVPOLL)
int io_master;
#endif

#if defined(HAVE_SIGIO)
int io_signum;
sigset_t io_ss;
#endif

#if defined(HAVE_SIGIO)
long alt_firstread;
long alt_firstwrite;
#endif

/* put d on internal data structure, return 1 on success, 0 on error */
static io_entry*
io_fd_internal(fd_t d, int flags) {
  io_entry* e;
#if !WINDOWS_NATIVE
  long r;
  if((flags & (IO_FD_BLOCK | IO_FD_NONBLOCK)) == 0) {
    if((r = fcntl(d, F_GETFL, 0)) == -1)
      return 0; /* file descriptor not open */
  } else if(flags & IO_FD_NONBLOCK)
    r = O_NDELAY;
  else
    r = 0;
#endif
  /* Problem: we might be the first to use io_fds. We need to make sure
   * we are the only ones to initialize it.  So set io_fds_inited to 2
   * and not to 1.  We know we are done when it is 1.  We know we need
   * to do something when it is 0.  We know somebody else is doing it
   * when it is 2. */
  if(__CAS(&io_fds_inited, 0, 2) == 0) {
    iarray_init(&io_fds, sizeof(io_entry));
    io_fds_inited = 1;
  } else
    do {
#ifdef __GNUC__
      __asm__("" : : : "memory");
#endif
    } while(io_fds_inited != 1);
  if(!(e = (io_entry*)iarray_allocate(&io_fds, (size_t)d)))
    return 0;
  if(e->inuse)
    return e;
  byte_zero(e, sizeof(io_entry));
  e->inuse = 1;
#if WINDOWS_NATIVE
  e->mh = 0;
#else
  if(r & O_NDELAY)
    e->nonblock = 1;
#endif
  e->next_read = e->next_write = -1;
  if(io_waitmode == UNDECIDED) {
    first_readable = first_writeable = -1;
#if defined(HAVE_EPOLL)
    io_master = epoll_create(1000);
    if(io_master != -1)
      io_waitmode = EPOLL;
#endif

#if defined(HAVE_KQUEUE)
    if(io_waitmode == UNDECIDED) { /* who knows, maybe someone supports both one day */
      io_master = kqueue();
      if(io_master != -1)
        io_waitmode = KQUEUE;
    }
#endif

#if defined(HAVE_DEVPOLL)
    if(io_waitmode == UNDECIDED) {
      io_master = open("/dev/poll", O_RDWR);
      if(io_master != -1)
        io_waitmode = DEVPOLL;
    }
#endif

#if defined(HAVE_SIGIO)
    alt_firstread = alt_firstwrite = -1;
    if(io_waitmode == UNDECIDED) {
      io_signum = SIGRTMIN + 1;
      if(sigemptyset(&io_ss) == 0 && sigaddset(&io_ss, io_signum) == 0 && sigaddset(&io_ss, SIGIO) == 0 &&
         sigprocmask(SIG_BLOCK, &io_ss, 0) == 0)
        io_waitmode = _SIGIO;
    }
#endif

#if WINDOWS_NATIVE && !defined(USE_SELECT)
    io_comport = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
    if(io_comport) {
      io_waitmode = COMPLETIONPORT;
    } else {
      errno = EINVAL;
      return 0;
    }
#endif
  }
#if defined(HAVE_SIGIO)
  if(io_waitmode == _SIGIO) {
    fcntl(d, F_SETOWN, getpid());
    fcntl(d, F_SETSIG, io_signum);
#if defined(O_ONESIGFD) && defined(F_SETAUXFL)
    fcntl(d, F_SETAUXFL, O_ONESIGFD);
#endif
    fcntl(d, F_SETFL, fcntl(d, F_GETFL) | O_NONBLOCK | O_ASYNC);
  }
#endif

#if WINDOWS_NATIVE && !defined(USE_SELECT)
  if(io_comport) {
    if(CreateIoCompletionPort((HANDLE)(UINT_PTR)d, io_comport, (UINT_PTR)d, 0) == 0) {
      errno = EBADF;
      return 0;
    }
  }
#endif
  return e;
}

int
io_fd(fd_t d) {
  io_entry* e = io_fd_internal(d, 0);
  return !!e;
}

int
io_fd_canwrite(fd_t d) {
  io_entry* e = io_fd_internal(d, 0);
  if(e)
    e->canwrite = 1;
  return !!e;
}

int
io_fd_flags(fd_t d, int flags) {
  io_entry* e = io_fd_internal(d, flags);
  if(e && (flags & IO_FD_CANWRITE))
    e->canwrite = 1;
  return !!e;
}

