#define _WINSOCKAPI_
#include "../socket.h"

#include "../socket_internal.h"
#if WINDOWS_NATIVE 
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
