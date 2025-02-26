#define USE_WS2_32 1
#include "../socket_internal.h"
#include <sys/types.h>

#if WINDOWS_NATIVE
//#define _WINSOCKAPI_
#define HAVE_N2I
/*#undef WS2TCPIP_INLINE
#define WS2TCPIP_INLINE static inline
#include <netioapi.h>*/
#else
#include <net/if.h>
#endif

#if WINDOWS_NATIVE
static uint32
if_nametoindex(const char* name) {

  typedef uint32(WINAPI ifn2idx_fn)(const char*);
  static ifn2idx_fn* ifn2idx;

  if(ifn2idx == 0) {
    HANDLE iphlpapi = LoadLibraryA("iphlpapi.dll");

    if(iphlpapi != INVALID_HANDLE_VALUE) {
      if((ifn2idx = (ifn2idx_fn*)GetProcAddress(iphlpapi, "if_nametoindex")) == 0)
        return -1;
    }
  }

  if(ifn2idx)
    return ifn2idx(name);
  return -1;
}
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
