#define USE_WS2_32 1

#if WINDOWS_NATIVE
#define _WINSOCKAPI_
#endif

#include "../socket_internal.h"
#include <sys/types.h>

#ifndef WINDOWS
#include <net/if.h>
#endif

uint32
socket_getifidx(const char* ifname) {
#if defined(HAVE_N2I) && !defined(__EMSCRIPTEN__)
  return if_nametoindex(ifname);
#else
  return 0;
#endif
}
