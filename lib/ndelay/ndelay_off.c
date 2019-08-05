#define USE_WS2_32 1
#include "../socket_internal.h"
#include "../ndelay.h"

#if !WINDOWS_NATIVE
#include <fcntl.h>
#include <sys/types.h>
#endif

#ifndef O_NDELAY
#define O_NDELAY O_NONBLOCK
#endif

int
ndelay_off(fd_t fd) {
#if WINDOWS_NATIVE
  unsigned long i = 1;
  return winsock2errno(ioctlsocket(fd, FIONBIO, &i));
#else
  return fcntl(fd, F_SETFL, fcntl(fd, F_GETFL, 0) & ~O_NDELAY);
#endif
}

