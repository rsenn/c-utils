#define USE_WS2_32 1
#include "../socket_internal.h"
#include <sys/types.h>

#if WINDOWS_NATIVE
//#define _WINSOCKAPI_
#define HAVE_N2I
/*#undef WS2TCPIP_INLINE
#define WS2TCPIP_INLINE static inline
#define IN6_ADDR_EQUAL IN6_ADDR_EQUAL_2
#include <netioapi.h>*/
#else
#include <net/if.h>
#endif

#ifdef HAVE_N2I
#ifndef IF_NAMESIZE
#define IF_NAMESIZE 16
#endif

#if WINDOWS_NATIVE
static char*
if_indextoname(uint32 idx, const char* name) {

  typedef char*(WINAPI idx2ifn_fn)(uint32, const char*);
  static idx2ifn_fn* idx2ifn;

  if(idx2ifn == 0) {
    HANDLE iphlpapi = LoadLibraryA("iphlpapi.dll");

    if(iphlpapi != INVALID_HANDLE_VALUE) {
      if((idx2ifn = (idx2ifn_fn*)GetProcAddress(iphlpapi,
                                                "if_indextoname")) == 0)
        return 0;
    }
  }
  if(idx2ifn)
    return idx2ifn(idx, name);
  return 0;
}
#endif

static char ifname[IF_NAMESIZE];

const char*
socket_getifname(uint32 _interface) {
  char* tmp = if_indextoname(_interface, ifname);
  if(tmp)
    return tmp;
  else
    return "[unknown]";
}
#else
const char*
socket_getifname(uint32 _interface) {
  return "[unknown]";
}
#endif
