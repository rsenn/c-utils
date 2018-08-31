#include <fcntl.h>
#include <sys/types.h>

#include "../ndelay.h"

#if defined(_WIN32) || defined(_WIN64)
#include "../windoze.h"
#include <winsock.h>
#endif

#ifndef O_NDELAY
#define O_NDELAY O_NONBLOCK
#endif

int
ndelay_on(fd_t fd) {
#if defined(_WIN32) || defined(_WIN64)
  unsigned long i = 0;
  return winsock2errno(ioctlsocket(fd, FIONBIO, &i));
#else
  return fcntl(fd, F_SETFL, fcntl(fd, F_GETFL, 0) | O_NDELAY);
#endif
}
