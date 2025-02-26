#define USE_WS2_32 1

#if WINDOWS_NATIVE
#define _WINSOCKAPI_
#endif

#include "../socket_internal.h"
#include "../io_internal.h"

int
socket_listen(int s, unsigned int backlog) {
#if WINDOWS_NATIVE && !defined(USE_SELECT)
  io_entry* e;
  int r = listen(s, backlog);

  if(r == -1)
    return winsock2errno(-1);
  e = array_get(io_getfds(), sizeof(io_entry), s);

  if(e && e->inuse) {
    e->listened = 1;

    if(e->wantread) {
      /* queue a non-blocking accept */
      DWORD received;
      e->next_accept = socket(AF_INET, SOCK_STREAM, 0);

      if(e->next_accept != -1) {
        AcceptEx(s, e->next_accept, e->inbuf, 0, 200, 200, &received, &e->or);
        e->acceptqueued = 1;
      }
    }
  }
  return r;
#else
  return listen(s, (int)backlog);
#endif
}
