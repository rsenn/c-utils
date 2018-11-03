#define USE_WS2_32 1

#if WINDOWS_NATIVE
#define _WINSOCKAPI_
#include <winsock2.h>
#endif

#include "../socket_internal.h"

#if WINDOWS_NATIVE  && !defined(__MINGW32__)
typedef int socklen_t;
#endif

int
socket_connected(int s) {
  struct sockaddr si;
  socklen_t sl = sizeof si;
  if(getpeername(s, &si, &sl))
    return 0;
  return 1;
}
