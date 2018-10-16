#define _WINSOCKAPI_
#include "../socket.h"
#include <sys/types.h>
#if !WINDOWS_NATIVE
#include <sys/socket.h>
#endif

void
socket_tryreservein(int s, int size) {
  while(size >= 1024) {
    if(winsock2errno(setsockopt(s, SOL_SOCKET, SO_RCVBUF, &size, sizeof size)) == 0) return;
    size -= (size >> 5);
  }
}
