#include "../windoze.h"
#define USE_WS2_32 1

#if WINDOWS
#define _WINSOCK2API_
#include <winsock2.h>
#endif

#include "../socket_internal.h"
#include <errno.h>

#ifndef ENOSYS
#define ENOSYS 1052
#endif /* ENOSYS */

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
