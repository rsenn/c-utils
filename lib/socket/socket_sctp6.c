#include "../windoze.h"

#include "../ndelay.h"
#include "../socket.h"

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
