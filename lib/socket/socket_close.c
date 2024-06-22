#define USE_WS2_32 1

#if WINDOWS_NATIVE
#define _WINSOCKAPI_
#endif

#include "../socket_internal.h"

int
socket_close(fd_type s) {
  return closesocket(s);
}
