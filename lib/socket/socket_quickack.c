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
socket_quickack(int s, int value) {
#ifdef TCP_QUICKACK
  return setsockopt(s, SOL_TCP, TCP_QUICKACK, (void*)&value, sizeof(int));
#else
#ifdef ENOPROTOOPT
  errno = ENOPROTOOPT;
#else
  errno = ENOSYS;
#endif
  return -1;
#endif
}
