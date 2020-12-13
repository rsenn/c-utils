#ifndef IO_INTERNAL_H
#define IO_INTERNAL_H 1

#include "windoze.h"

#ifndef my_extern
#if WINDOWS
#define my_extern extern __declspec(dllexport)
#else
#define my_extern extern
#endif
#endif

#include "array.h"
#include "iarray.h"
#include "taia.h"
#include "io.h"

extern void* io_getfds();

#ifdef HAVE_LINUX_AIO
#include <linux/aio_abi.h>
#endif

#if WINDOWS_NATIVE
#ifdef _MSC_VER
#define _CRT_INTERNAL_NONSTDC_NAMES 1
#endif
#include <io.h>
#endif

#if WINDOWS
#if WINDOWS_NATIVE
#include <windows.h>
#if !defined(__LCC__) && !defined(__MINGW32__) && !defined(__MSYS__)
#define read _read
#define write _write
#define open _open
#define close _close
#endif
#if !defined(__DMC__)
#define popen _popen
#endif
#endif
//#define lseek lseek64

my_extern intptr_t io_comport;

#elif !defined(__MSYS__) && !defined(__CYGWIN__) && !defined(_WIN32) && !defined(__APPLE__)
//# define HAVE_EPOLL 1
//# define HAVE_SIGIO 1

#ifdef HAVE_SIGIO
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#undef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 199309L
#include <signal.h>
#endif

#endif /* WINDOWS_NATIVE */

#if !defined(WINDOWS)
#if defined(__unix__) || defined(__linux__)
#include <sys/time.h>
#include <unistd.h>
#endif
#endif

#include <errno.h>

#ifdef HAVE_KQUEUE
#include <sys/event.h>
#endif

#ifdef HAVE_DEVPOLL
#include <sys/devpoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#endif

#ifdef HAVE_EPOLL
#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE
#endif

#include "byte.h"
#include <inttypes.h>
#include <sys/epoll.h>

#ifndef EPOLLRDNORM
#define EPOLLRDNORM 0
#endif

#ifndef EPOLLRDBAND
#define EPOLLRDNORM 0
#endif
#endif
#if USE_LINUX_AIO
#include <setjmp.h>
#endif
#if defined(__MINGW32__) || defined(__MINGW64__)
int write();
int read();
#endif

/* We simulate a level-triggered API on top of an event signalling
 * mechanism that can be level-triggered (epoll/kqueue/poll) or
 * edge-triggered (SIGIO).
 * Difficulty: we want to avoid unnecessary syscalls, so we keep state
 * internally.  If the user says he does not want to read/write anymore,
 * we don't tell the kernel straight away.  The rationale is that the
 * typical protocol consists of interleaved reading and writing, so
 * after each read you'd call io_dontwantread, io_wantwrite, io_wait,
 * io_dontwantwrite, io_wantread, and in the regular case there is no
 * incoming data between io_dontwantread and io_wantread, so we might as
 * well optimistically not do those syscalls and then handle the
 * complexity if there is more incoming data. */

/* Basically, we tell the kernel that we want to read if !canread,
 * and we tell the kernel that we want to write if !canwrite. */

typedef struct ioent {
  tai6464 timeout;
  unsigned int wantread : 1; /* does the app want to read/write? */
  unsigned int wantwrite : 1;
  unsigned int canread : 1; /* do we know we can read/write? */
  unsigned int canwrite : 1;
  unsigned int nonblock : 1;       /* is this socket non-blocking? */
  unsigned int inuse : 1;          /* internal consistency checking */
  unsigned int kernelwantread : 1; /* did we tell the kernel we want to read/write? */
  unsigned int kernelwantwrite : 1;
  unsigned int epolladded : 1;
#if WINDOWS_NATIVE //&& !defined(USE_SELECT)
  unsigned int readqueued : 2;
  unsigned int writequeued : 2;
  unsigned int acceptqueued : 2;
  unsigned int connectqueued : 2;
  unsigned int sendfilequeued : 2;
  unsigned int listened : 2;
#endif
  long next_read;
  long next_write;
  void* cookie;
  void* mmapped;
  long maplen;
  uint64 mapofs;
#if USE_LINUX_AIO
  struct iocb cb;
  jmp_buf jmp;
#endif
#if WINDOWS_NATIVE
  HANDLE /* fd, */ mh;
#if 1
  OVERLAPPED or, ow, os; /* overlapped for read+accept, write+connect, sendfile */
  int bytes_read, bytes_written;
  DWORD errorcode;
  int64 next_accept;
#endif
#endif
#if defined(USE_LINUX_AIO) || WINDOWS_NATIVE
  char inbuf[8192];
#endif
} io_entry;

extern int io_multithreaded;
extern int io_sockets[2];

my_extern uint64 io_wanted_fds;
my_extern array io_pollfds;

my_extern long first_readable;
my_extern long first_writeable;

my_extern enum __io_waitmode {
  UNDECIDED,
  POLL
#ifdef HAVE_KQUEUE
  ,
  KQUEUE
#endif
#ifdef HAVE_EPOLL
  ,
  EPOLL
#endif
#ifdef HAVE_SIGIO
  ,
  _SIGIO
#endif
#ifdef HAVE_DEVPOLL
  ,
  DEVPOLL
#endif
#if WINDOWS
  ,
  COMPLETIONPORT
#endif
} io_waitmode;

#if defined(HAVE_KQUEUE) || defined(HAVE_EPOLL) || defined(HAVE_DEVPOLL)
my_extern int io_master;
#endif
#if defined(HAVE_SIGIO)
my_extern int io_signum;
my_extern sigset_t io_ss;

my_extern long alt_firstread;
my_extern long alt_firstwrite;
#endif

int64 io_waituntil2(int64 milliseconds);

void io_sigpipe(void);

/* return next descriptor from io_wait that can be read from */
int64 io_canread_unlocked();
/* return next descriptor from io_wait that can be written to */
int64 io_canwrite_unlocked();
/* return next descriptor with expired timeout */
int64 io_timeouted_unlocked();

struct eventpacket {
  fd_t fd;
  enum { CANREAD, CANWRITE, TIMEOUT } what;
};

#if WINDOWS_NATIVE && !__POCC__
// extern int open();
extern int read();
extern int write();
extern int close();
#endif

#define debug_printf(x)

#ifdef DEBUG
#define DEBUG_MSG(msg, fd) buffer_puts(buffer_2, msg), buffer_putlong(buffer_2, fd), buffer_putnlflush(buffer_2)
#else
#define DEBUG_MSG(msg, fd)
#endif

#if defined(HAVE_SYSCALL) && !WINDOWS
#include <sys/syscall.h>

static inline ssize_t
sys_write(int fd, const void* buf, size_t n) {
  return syscall(SYS_write, fd, buf, n);
}
#define write sys_write

#elif defined(HAVE___WRITE)
ssize_t __write(int, const void*, size_t);
#define write __write
#endif

#ifdef HAVE_LINUX_AIO
#include <linux/aio_abi.h>
#include <sys/syscall.h>
#ifdef __dietlibc__
#include <bits/syscall.h>
#endif

#define IOCB_CMD_PREADX 4
#define IOCB_CMD_POLL 5
#ifndef __dietlibc__
#ifndef HAVE_IO_SETUP
static inline int
io_setup(unsigned nr, aio_context_t* ctxp) {
  return syscall(__NR_io_setup, nr, ctxp);
}
#endif
#ifndef HAVE_IO_DESTROY
static inline int
io_destroy(aio_context_t ctx) {
  return syscall(__NR_io_destroy, ctx);
}
#endif
#ifndef HAVE_IO_SUBMIT
static inline int
io_submit(aio_context_t ctx, long nr, struct iocb** iocbpp) {
  return syscall(__NR_io_submit, ctx, nr, iocbpp);
}
#endif
#ifndef HAVE_IO_GETEVENTS
static inline int
io_getevents(aio_context_t ctx, long min_nr, long max_nr, struct io_event* events, struct timespec* timeout) {
  return syscall(__NR_io_getevents, ctx, min_nr, max_nr, events, timeout);
}
#endif
#endif /* defined __dietlibc__ */
#endif
#endif

#ifdef DEBUG_IO
#include "buffer.h"
#endif
