#define _WINSOCKAPI_
#include "../socket.h"
#include <sys/types.h>
#if !WINDOWS_NATIVE
#include <net/if.h>
#include <sys/socket.h>
#endif

uint32
socket_getifidx(const char* ifname) {
#ifdef HAVE_N2I
  return if_nametoindex(ifname);
#else
  return 0;
#endif
}
