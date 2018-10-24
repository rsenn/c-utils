#ifndef IO_INTERNAL_H
#define IO_INTERNAL_H 1

#include "windoze.h"

#ifndef my_extern
# if WINDOWS
#  define my_extern extern __declspec(dllexport)
# else
#  define my_extern extern
# endif
#endif

#include "array.h"
#include "iarray.h"
#include "taia.h"
#include "io.h"

extern void* io_getfds();

#if WINDOWS_NATIVE
# include <io.h>

#endif

#if WINDOWS
# include <windows.h>
//#define read _read
//#define write _write
//#define open _open
//#define close _close
# define popen _popen
//#define lseek lseek64
# define llseek lseek64
my_extern HANDLE io_comport;

# elif !defined(__MSYS__) && !defined(__CYGWIN__) && !defined(_WIN32) && !defined(__APPLE__)
//#define HAVE_EPOLL 1
# define HAVE_SIGIO 1

# ifdef HAVE_SIGIO
#  ifndef _GNU_SOURCE
#   define _GNU_SOURCE
#  endif
#  undef _POSIX_C_SOURCE
#  define _POSIX_C_SOURCE  199309L
#  include <signal.h>
# endif

#endif /* WINDOWS_NATIVE */

#if !defined(WINDOWS)
#if defined(__unix__) || defined(__linux__)
#include <sys/time.h>
#include <unistd.h>
#endif
#endif

#include <errno.h>

#ifndef io_seek
#warning No io_seek() function defined, defaulting to lseek
#define io_seek lseek
#endif

#ifndef STDIN_FILENO
#define STDIN_FILENO 0
#endif
#ifndef STDOUT_FILENO
#define STDOUT_FILENO 1
#endif
#ifndef STDERR_FILENO
#define STDERR_FILENO 2
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

typedef struct {
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
#if WINDOWS
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
#if WINDOWS
  OVERLAPPED or, ow, os; /* overlapped for read+accept, write+connect, sendfile */
  HANDLE /* fd, */ mh;
  char inbuf[8192];
  int bytes_read, bytes_written;
  DWORD errorcode;
  int64 next_accept;
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

#if WINDOWS_NATIVE
//extern int open();
extern int read();
extern int write();
extern int close();
#endif

#define debug_printf(x)

#ifdef DEBUG
#define DEBUG_MSG(msg, fd) \
  buffer_puts(buffer_2, msg), \
  buffer_putlong(buffer_2, fd), \
  buffer_putnlflush(buffer_2)
#else
#define DEBUG_MSG(msg, fd)
#endif

#endif

