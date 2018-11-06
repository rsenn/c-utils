#define USE_WS2_32 1

#if WINDOWS
#define _WINSOCKAPI_
#endif

#include "../socket_internal.h"
#include <errno.h>

#ifndef ENOSYS
#define ENOSYS 1052
#endif /* ENOSYS */

int
socket_fastopen(int s) {
#if defined(SOL_TCP) && defined(TCP_FASTOPEN)
  int opt = 5;
  return setsockopt(s, SOL_TCP, TCP_FASTOPEN, (void*)&opt, sizeof(int));
#else
#ifdef ENOPROTOOPT
  errno = ENOPROTOOPT;
#else
  errno = ENOSYS;
#endif
  return -1;
#endif
}
