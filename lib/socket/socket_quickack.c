#define _WINSOCKAPI_
#include "../socket.h"
#if !WINDOWS_NATIVE
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <sys/types.h>
#endif
#include <errno.h>

int
socket_quickack(int s, int value) {
#ifdef TCP_QUICKACK
  return setsockopt(s, SOL_TCP, TCP_QUICKACK, &value, sizeof(int));
#else
#ifdef ENOPROTOOPT
  errno = ENOPROTOOPT;
#else
  errno = ENOSYS;
#endif
  return -1;
#endif
}
