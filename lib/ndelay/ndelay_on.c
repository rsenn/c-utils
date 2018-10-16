#define _WINSOCKAPI_
#include "../socket.h"
#include "../ndelay.h"
#if WINDOWS_NATIVE
#else
#include <fcntl.h>
#include <sys/types.h>
#endif

#ifndef O_NDELAY
#define O_NDELAY O_NONBLOCK
#endif

int
ndelay_on(fd_t fd) {
#if WINDOWS_NATIVE
  unsigned long i = 0;
  return winsock2errno(ioctlsocket(fd, FIONBIO, &i));
#else
  return fcntl(fd, F_SETFL, fcntl(fd, F_GETFL, 0) | O_NDELAY);
#endif
}
