#define USE_WS2_32 1
#include "../socket_internal.h"
#include <sys/types.h>
#include "../ndelay.h"

int
socket_udp4(void) {
  int s;
  s = socket_udp4b();

  if(s != -1 && ndelay_on(s) == -1) {
    closesocket(s);
    return -1;
  }
  return s;
}
