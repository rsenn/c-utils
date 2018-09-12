#include "../socket_internal.h"
#include "../windoze.h"
#if WINDOWS_NATIVE 
#include <winsock.h>
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
