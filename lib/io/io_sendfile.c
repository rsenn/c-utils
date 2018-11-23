#define USE_WS2_32 1
#define WINDOWS_OVERRIDE 1
#include "../windoze.h"
#include "../socket_internal.h"

#if WINDOWS
#include <mswsock.h>
#endif

/* http://delegate.uec.ac.jp:8081/club/mma/~shimiz98/misc/sendfile.html */

#define _LARGEFILE64_SOURCE
#define _FILE_OFFSET_BITS 64
#include "../io_internal.h"

#ifdef HAVE_ERRNO_H
#include <errno.h>
#endif

#ifdef __aarch64__
#include <asm/unistd.h>
#endif
#include "../uint8.h"

#if defined(HAVE_BSDSENDFILE)
#define SENDFILE 1

int64
io_sendfile(fd_t s, fd_t fd, uint64 off, uint64 n) {
  off_t sbytes;
  int r = sendfile(fd, s, off, n, 0, &sbytes, 0);
  if(r == -1) {
    io_entry* e = iarray_get(io_getfds(), s);
    if(e) {
      e->canwrite = 0;
      e->next_write = -1;
    }
    return errno == EAGAIN ? (sbytes ? sbytes : -1) : -3;
  }
  return n;
}

#elif defined(HAVE_SENDFILE)

#ifdef __hpux__

int64
io_sendfile(int64 out, int64 in, uint64 off, uint64 bytes) {
  long long r = sendfile64(out, in, off, bytes, 0, 0);
  if(r == -1 && errno != EAGAIN)
    r = -3;
  if(r != bytes) {
    io_entry* e = iarray_get(io_getfds(), s);
    if(e) {
      e->canwrite = 0;
      e->next_write = -1;
    }
  }
  return r;
}

#elif defined(__sun__) && defined(__svr4__)

#include <sys/sendfile.h>

int64
io_sendfile(int64 out, int64 in, uint64 off, uint64 bytes) {
  off64_t o = off;
  long long r = sendfile64(out, in, &o, bytes);
  if(r == -1 && errno != EAGAIN)
    r = -3;
  if(r != bytes) {
    io_entry* e = iarray_get(io_getfds(), s);
    if(e) {
      e->canwrite = 0;
      e->next_write = -1;
    }
  }
  return r;
}

#elif defined(_AIX)

int64
io_sendfile(int64 out, int64 in, uint64 off, uint64 bytes) {
  struct sf_parms p;
  int64 destfd = out;
  p.header_data = 0;
  p.header_length = 0;
  p.file_descriptor = in;
  p.file_offset = off;
  p.file_bytes = bytes;
  p.trailer_data = 0;
  p.trailer_length = 0;
  if(send_file(&destfd, &p, 0) >= 0) {
    if(p.bytes_sent != bytes) {
      io_entry* e = iarray_get(io_getfds(), s);
      if(e) {
        e->canwrite = 0;
        e->next_write = -1;
      }
    }
    return p.bytes_sent;
  }
  if(errno == EAGAIN)
    return -1;
  else
    return -3;
}

#elif defined(__linux__)

#if defined(__GLIBC__)
#include <sys/sendfile.h>
#elif defined(__dietlibc__)
#include <sys/sendfile.h>
#else
#ifndef __NR_sendfile
#define __NR_sendfile 71
#endif

#ifdef __aarch64__
#define sendfile(x...) syscall(__NR_sendfile64, x)
#else
#define sendfile(x...) syscall(__NR_sendfile, x)
//_syscall4(int, sendfile, int, out, int, in, long*, offset, unsigned long, count)
#endif

#endif

int64
io_sendfile(fd_t s, fd_t fd, uint64 off, uint64 n) {
  off_t o = off;
  io_entry* e = iarray_get(io_getfds(), s);
  off_t i;
  uint64 done = 0;
  /* What a spectacularly broken design for sendfile64.
   * The offset is 64-bit for sendfile64, but the count is not. */
  while(n) {
    off_t todo = n > 0x7fffffff ? 0x7fffffff : n;
    i = sendfile(s, fd, &o, todo);
    if(i == todo) {
      done += todo;
      n -= todo;
      if(n == 0)
        return done;
      continue;
    } else {
      if(e) {
        e->canwrite = 0;
        e->next_write = -1;
      }
      if(i == -1)
        return errno == EAGAIN ? -1 : -3;
      else
        return done + i;
    }
  }
  return 0;
}
#endif

#elif(defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)) && !defined(__MSYS__)

//#undef closesocket
//#include <winsock2.h>
//#include <windows.h>

#ifndef TF_USE_KERNEL_APC
#define TF_USE_KERNEL_APC 0x20
#endif

int64
io_sendfile(fd_t out, fd_t in, uint64 off, uint64 bytes) {
#ifdef USE_SELECT
  return -1;
#else
  typedef BOOL WINAPI transmit_file_fn(SOCKET, HANDLE, DWORD, DWORD, LPOVERLAPPED, LPTRANSMIT_FILE_BUFFERS, DWORD);
  static transmit_file_fn* transmit_file;

  io_entry* e = iarray_get(io_getfds(), out);
  if(!e) {
    errno = EBADF;
    return -3;
  }
  if(transmit_file == 0) {
    HANDLE wsock32 = LoadLibraryA("mswsock.dll");
    if(wsock32 == INVALID_HANDLE_VALUE)
      wsock32 = LoadLibraryA("wsock32.dll");

    if(wsock32 != INVALID_HANDLE_VALUE) {
      transmit_file = (transmit_file_fn*)GetProcAddress(wsock32, "TransmitFile");
    }
  }

  if(transmit_file == 0)
    return -1;

  if(e->sendfilequeued == 1) {
    /* we called TransmitFile, and it returned. */
    e->sendfilequeued = 2;
    errno = e->errorcode;
    if(e->bytes_written == -1)
      return -1;
    if(e->bytes_written != bytes) { /* we wrote less than caller wanted to write */
      e->sendfilequeued = 1;        /* so queue next request */
      off += e->bytes_written;
      bytes -= e->bytes_written;
      e->os.Offset = off;
      e->os.OffsetHigh = (off >> 32);
      (*transmit_file)((SOCKET)out, (HANDLE)in, bytes > 0xffff ? 0xffff : bytes, 0, &e->os, 0, TF_USE_KERNEL_APC);
    }
    return e->bytes_written;
  } else {
    e->sendfilequeued = 1;
    e->os.Offset = off;
    e->os.OffsetHigh = (off >> 32);
    /* we always write at most 64k, so timeout handling is possible */
    if(!(*transmit_file)((SOCKET)out, (HANDLE)in, bytes > 0xffff ? 0xffff : bytes, 0, &e->os, 0, TF_USE_KERNEL_APC))
      return -3;
  }
  return e->bytes_written;
#endif
}

#else

#include "../iob.h"

static int64
writecb(fd_t s, const void* buf, uint64 n) {
  return write(s, buf, n);
}

int64
io_sendfile(fd_t out, fd_t in, uint64 off, uint64 bytes) {
  return io_mmapwritefile(out, in, off, bytes, writecb);
}

#endif
