#include <sys/types.h>
#if !defined(_WIN32) && !defined(_WIN64)
#include <sys/socket.h>
#endif
#include "../socket.h"
#include "../windoze.h"

void
socket_tryreservein(int s, int size) {
  while(size >= 1024) {
    if(winsock2errno(setsockopt(s, SOL_SOCKET, SO_RCVBUF, &size, sizeof size)) == 0) return;
    size -= (size >> 5);
  }
}
