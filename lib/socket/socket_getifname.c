#define USE_WS2_32 1
#include "../socket_internal.h"
#include <sys/types.h>

#if WINDOWS_NATIVE
//#define _WINSOCKAPI_
#define HAVE_N2I
#undef WS2TCPIP_INLINE 
#define WS2TCPIP_INLINE static inline
#define IN6_ADDR_EQUAL IN6_ADDR_EQUAL_2
#include <netioapi.h>
#else
#include <net/if.h>
#endif


#ifdef HAVE_N2I
/* legacy BSD name */
#ifndef IF_NAMESIZE
#define IF_NAMESIZE IFNAMSIZ
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
