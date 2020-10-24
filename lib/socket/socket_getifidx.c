#define USE_WS2_32 1
#include "../socket_internal.h"
#include <sys/types.h>

#if WINDOWS_NATIVE
//#define _WINSOCKAPI_
#define HAVE_N2I
#undef WS2TCPIP_INLINE 
#define WS2TCPIP_INLINE static inline
#include <netioapi.h>
#else
#include <net/if.h>
#endif

#if !defined(__wasi__)

uint32
socket_getifidx(const char* ifname) {
#if defined(HAVE_N2I) && !defined(__EMSCRIPTEN__)
  return if_nametoindex(ifname);
#else
  return 0;
#endif
}
#endif
