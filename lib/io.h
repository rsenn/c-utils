/**
 * @defgroup   io
 * @brief      IO module.
 * @{
 */
/* this header file comes from libowfat, http://www.fefe.de/libowfat/ */
#ifndef IO_H
#define IO_H

/* http://cr.yp.to/lib/io.html */

#include "windoze.h"
#include "iarray.h"
#include "taia.h"
#include "uint64.h"
#include "typedefs.h"
#include "config.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef IO_INTERNAL_H
typedef struct ioent io_entry;
#endif

/* like open(s,O_RDONLY) */
/* return 1 if ok, 0 on error */
int io_readfile(fd_type* pfd, const char* s);

/* like open(s,O_WRONLY|O_CREAT|O_TRUNC,0600) */
/* return 1 if ok, 0 on error */

int io_createfile(fd_type* pfd, const char* s);

/* like open(s,O_RDWR) */
/* return 1 if ok, 0 on error */
int io_readwritefile(fd_type* pfd, const char* s);

/* like open(s,O_WRONLY|O_APPEND|O_CREAT,0600) */
/* return 1 if ok, 0 on error */
int io_appendfile(fd_type* pfd, const char* s);

/* like pipe(d) */
/* return 1 if ok, 0 on error */

int io_pipe(fd_type* pfd);

/* like socketpair() */
/* return 1 if ok, 0 on error */
int io_socketpair(fd_type* pfd);

/* non-blocking read(), -1 for EAGAIN and -3+errno for other errors */
int64 io_tryread(fd_type d, char* buf, int64 len);

/* blocking read(), with -3 instead of -1 for errors */
int64 io_waitread(fd_type d, char* buf, int64 len);

/* non-blocking write(), -1 for EAGAIN and -3+errno for other errors */
int64 io_trywrite(fd_type d, const char* buf, int64 len);

/* blocking write(), with -3 instead of -1 for errors */
int64 io_waitwrite(fd_type d, const char* buf, int64 len);

/* modify timeout attribute of file descriptor */
void io_timeout(fd_type d, tai6464 t);

/* like io_tryread but will return -2,errno=ETIMEDOUT if d has a timeout
 * associated and it is passed without input being there */
int64 io_tryreadtimeout(fd_type d, char* buf, int64 len);

/* like io_trywrite but will return -2,errno=ETIMEDOUT if d has a timeout
 * associated and it is passed without being able to write */
int64 io_trywritetimeout(fd_type d, const char* buf, int64 len);

void io_wantread(fd_type d);
void io_wantwrite(fd_type d);
void io_dontwantread(fd_type d);
void io_dontwantwrite(fd_type d);

void io_onlywantread(fd_type);
void io_onlywantwrite(fd_type);

int io_get_wantread(fd_type d);
int io_get_wantwrite(fd_type d);

int64 io_wait();
void io_waituntil(tai6464 t);
int64 io_waituntil2(int64 milliseconds);
void io_check();

/* signal that read/accept/whatever returned EAGAIN */
/* needed for SIGIO and epoll */
void io_eagain(fd_type d); /* do not use, API was a bad idea */
#define HAVE_EAGAIN_READWRITE
void io_eagain_read(fd_type d); /* use these ones */
void io_eagain_write(fd_type d);

/* return next descriptor from io_wait that can be read from */
int64 io_canread();
/* return next descriptor from io_wait that can be written to */
int64 io_canwrite();

/* return next descriptor with expired timeout */
int64 io_timeouted();

/* is this fd over its timeout? */
int io_timedout(fd_type d);

/* 1 means: have IO_FD_CANWRITE, IO_FD_BLOCK and IO_FD_NONBLOCK,
 * will be incremented if API is extended in the future */
#ifndef HAVE_IO_FD_FLAGS
#define HAVE_IO_FD_FLAGS 1
enum io_fd_flags {
  IO_FD_CANWRITE = 1, /* new TCP connection, we know it's writable */
  IO_FD_BLOCK = 2,    /* skip the fcntl, assume fd is set to blocking */
  IO_FD_NONBLOCK = 4, /* skip the fcntl, assume fd is set to non-blocking */
};
#endif

/* put d on internal data structure, return 1 on success, 0 on error */
int io_fd(fd_type d);                  /* use this for sockets before you called connect()
                                          or accept() */
int io_fd_canwrite(fd_type d);         /* use this for connected sockets (assumes
                                          socket is writable) */
int io_fd_flags(fd_type d, int flags); /* can be used to tell io_fd to skip one syscall */

void io_setcookie(fd_type d, void* cookie);
void* io_getcookie(fd_type d);

/* put descriptor in non-blocking mode */
void io_nonblock(fd_type d);
/* put descriptor in blocking mode */
void io_block(fd_type d);
/* put descriptor in close-on-exec mode */
void io_closeonexec(fd_type d);

void io_close(fd_type d);

/* Free the internal data structures from libio.
 * This only makes sense if you run your program in a malloc checker and
 * these produce false alarms.  Your OS will free these automatically on
 * process termination. */
void io_finishandshutdown(void);

/* send n bytes from file fd starting at offset off to socket s */
/* return number of bytes written */
int64 io_sendfile(fd_type s, fd_type fd, uint64 off, uint64 n);

/* Pass fd over sock (must be a unix domain socket) to other process.
 * Return 0 if ok, -1 on error, setting errno. */
int io_passfd(fd_type sock, fd_type fd);

/* Receive fd over sock (must be a unix domain socket) from other
 * process.  Return sock if ok, -1 on error, setting errno. */
int64 io_receivefd(fd_type sock);

int io_starteventloopthread(unsigned int threads);

#define HAVE_IO_QUEUEFORREAD
/* Artificially queue a file descriptor as readable.
 * The next call to io_canread will return this descriptor. */
int io_queueforread(fd_type d);
/* Artificially queue a file descriptor as writable.
 * The next call to io_canread will return this descriptor. */
int io_queueforwrite(fd_type d);

typedef int64 (*io_write_callback)(fd_type sfd, const void* buf, uint64 n);

/* used internally, but hey, who knows */
int64 io_mmapwritefile(fd_type out, fd_type in, uint64 off, uint64 bytes, io_write_callback writecb);

/* only needed for debugging, will print some stats into the buffer to
 * aid in debugging the state machine if a descriptor loops or so */
unsigned int io_debugstring(fd_type sfd, char* buf, unsigned int bufsize);

void* io_getfds(void);
#define io_getentry(fd) ((io_entry*)iarray_get((iarray*)io_getfds(), (fd)))

#ifndef io_seek
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

#ifndef IOPAUSE_POLL
#ifdef HAVE_POLL
#define IOPAUSE_POLL
#endif
#endif

#ifdef BUFFER_H
void io_dump(buffer*);
#endif

#ifdef __cplusplus
}
#endif

#endif /* defined IO_H */
/** @} */
