#define USE_WS2_32 1

#if WINDOWS
#define _WINSOCKAPI_
#include <winsock2.h>
#endif

#include "../socket_internal.h"
#include "../ndelay.h"

int
socket_sctp6(void) {
  int s = socket_sctp6b();
  if(s == -1) return -1;
  if(ndelay_on(s) == -1) {
    closesocket(s);
    return -1;
  }
  return s;
}
