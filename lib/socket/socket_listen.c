#include "../windoze.h"
#if WINDOWS_NATIVE 
#include <winsock.h>
#else
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#endif
#include "../io_internal.h"

int
socket_listen(int s, unsigned int backlog) {
#if WINDOWS_NATIVE
  io_entry* e;
  int r = listen(s, backlog);
  if(r == -1) return winsock2errno(-1);
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
