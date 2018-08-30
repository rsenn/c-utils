#include "../socket.h"
#if !defined(_WIN32) && !defined(_WIN64)
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <sys/types.h>
#endif
#include <errno.h>

int
socket_fastopen(int s) {
#if defined(SOL_TCP) && defined(TCP_FASTOPEN)
  return setsockopt(s, SOL_TCP, TCP_FASTOPEN, (int[]){5}, sizeof(int));
#else
#ifdef ENOPROTOOPT
  errno = ENOPROTOOPT;
#else
  errno = ENOSYS;
#endif
  return -1;
#endif
}
