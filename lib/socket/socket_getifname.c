#define _WINSOCKAPI_
#include "../socket.h"
#include <sys/types.h>
#if !WINDOWS_NATIVE
#include <net/if.h>
#include <sys/socket.h>
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
