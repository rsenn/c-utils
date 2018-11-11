#define USE_WS2_32 1

#if WINDOWS_NATIVE
#define _WINSOCKAPI_
#endif

#include "../socket_internal.h"
#include <sys/types.h>

uint32
socket_getifidx(const char* ifname) {
#ifdef HAVE_N2I
  return if_nametoindex(ifname);
#else
  return 0;
#endif
}
