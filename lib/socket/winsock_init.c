#define _WINSOCKAPI_
#include "../socket.h"


#if WINDOWS_NATIVE

void
__winsock_init(void) {
  static int inited;
  if(!inited) {
    WSADATA wsaData;
    if(WSAStartup(MAKEWORD(2, 2), &wsaData) || LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
      exit(111);
    inited = 1;
  }
}
#endif
