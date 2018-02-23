/* this header file comes from libowfat, http://www.fefe.de/libowfat/ */
#ifndef IO_H
#define IO_H

/* http://cr.yp.to/lib/io.html */

#include "uint64.h"
#include "taia.h"

#ifdef __cplusplus
extern "C" {
#endif

#define HAVE_PREAD
#define HAVE_SENDFILE
#define HAVE_UINT128

/* like open(s,O_RDONLY) */
/* return 1 if ok, 0 on error */
int io_readfile(int64* d,const char* s);
/* like open(s,O_WRONLY|O_CREAT|O_TRUNC,0600) */
/* return 1 if ok, 0 on error */
int io_createfile(int64* d,const char* s);
/* like open(s,O_RDWR) */
/* return 1 if ok, 0 on error */
int io_readwritefile(int64* d,const char* s);
/* like open(s,O_WRONLY|O_APPEND|O_CREAT,0600) */
/* return 1 if ok, 0 on error */
int io_appendfile(int64* d,const char* s);
/* like pipe(d) */
/* return 1 if ok, 0 on error */
int io_pipe(int64* d);
/* like socketpair() */
/* return 1 if ok, 0 on error */
int io_socketpair(int64* d);

/* non-blocking read(), -1 for EAGAIN and -3+errno for other errors */
int64 io_tryread(int64 d,char* buf,int64 len);

/* blocking read(), with -3 instead of -1 for errors */
int64 io_waitread(int64 d,char* buf,int64 len);

/* non-blocking write(), -1 for EAGAIN and -3+errno for other errors */
int64 io_trywrite(int64 d,const char* buf,int64 len);

/* blocking write(), with -3 instead of -1 for errors */
int64 io_waitwrite(int64 d,const char* buf,int64 len);

/* modify timeout attribute of file descriptor */
void io_timeout(int64 d,tai6464 t);

/* like io_tryread but will return -2,errno=ETIMEDOUT if d has a timeout
 * associated and it is passed without input being there */
int64 io_tryreadtimeout(int64 d,char* buf,int64 len);

/* like io_trywrite but will return -2,errno=ETIMEDOUT if d has a timeout
 * associated and it is passed without being able to write */
int64 io_trywritetimeout(int64 d,const char* buf,int64 len);

void io_wantread(int64 d);
void io_wantwrite(int64 d);
void io_dontwantread(int64 d);
void io_dontwantwrite(int64 d);

void io_wait();
void io_waituntil(tai6464 t);
int64 io_waituntil2(int64 milliseconds);
void io_check();

/* signal that read/accept/whatever returned EAGAIN */
/* needed for SIGIO and epoll */
void io_eagain(int64 d);  /* do not use, API was a bad idea */
#define HAVE_EAGAIN_READWRITE
void io_eagain_read(int64 d);	/* use these ones */
void io_eagain_write(int64 d);

/* return next descriptor from io_wait that can be read from */
int64 io_canread();
/* return next descriptor from io_wait that can be written to */
int64 io_canwrite();

/* return next descriptor with expired timeout */
int64 io_timeouted();

/* is this fd over its timeout? */
int io_timedout(int64 d);

/* 1 means: have IO_FD_CANWRITE, IO_FD_BLOCK and IO_FD_NONBLOCK,
 * will be incremented if API is extended in the future */
#define HAVE_IO_FD_FLAGS 1
enum io_fd_flags {
  IO_FD_CANWRITE=1,	/* new TCP connection, we know it's writable */
  IO_FD_BLOCK=2,	/* skip the fcntl, assume fd is set to blocking */
  IO_FD_NONBLOCK=4,	/* skip the fcntl, assume fd is set to non-blocking */
};

/* put d on internal data structure, return 1 on success, 0 on e