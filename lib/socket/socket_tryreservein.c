#define USE_WS2_32 1

#if WINDOWS
#define _WINSOCKAPI_
#endif

#include "../socket_internal.h"
#include <sys/types.h>

void
socket_tryreservein(int s, int size) {
  while(size >= 1024) {
    if(winsock2errno(setsockopt(s, SOL_SOCKET, SO_RCVBUF, &size, sizeof size)) == 0) return;
    size -= (size >> 5);
  }
}
