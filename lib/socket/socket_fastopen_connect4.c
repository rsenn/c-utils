#define USE_WS2_32 1

#if WINDOWS_NATIVE
#define _WINSOCKAPI_
#endif

#include "../socket_internal.h"
#include <errno.h>

#if WINDOWS_NATIVE
#include <io.h>
#else
#include <unistd.h>
#endif

int
socket_fastopen_connect4(int s, const char* ip, uint16 port, const char* buf, size_t len) {
#ifndef MSG_FASTOPEN
  int r;
  {
#else
  int r = socket_send4_flag(s, buf, len, ip, port, MSG_FASTOPEN);

  if(r == -1 && errno == ENOTCONN) {
#endif
    /* apparently the kernel does not support TCP fast open */
    r = socket_connect4(s, ip, port);

    if(r == 0)
      return write(s, buf, len);
  }
  return r;
}
