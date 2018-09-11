#if defined(_WIN32) || defined(_WIN64) || defined(__MSYS__) || defined(__CYGWIN__)
#include <winsock2.h>
#endif
#include <sys/types.h>
#if defined(_WIN32) || defined(_WIN64)
#include "../io_internal.h"
#include <mswsock.h>
#else
#include <sys/socket.h>
#endif
#include "../socket.h"
#include "../windoze.h"

int
socket_listen(int s, unsigned int backlog) {
#if defined(_WIN32) || defined(_WIN64)
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
