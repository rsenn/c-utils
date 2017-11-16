#include "http.h"
#include <errno.h>
#include <sys/socket.h>

void
http_readable(http* h) {
  if(h->response) {
    char recvbuf[8192];
    ssize_t  ret;
    http_response* r = h->response;

    for(;;) {
      ret = recv(h->sock, recvbuf, sizeof(recvbuf), 0);

      if(ret == -1 && errno == EAGAIN) {
        errno = 0;
        return;
      }

      if(ret > 0) {
        stralloc_catb(&r->body, recvbuf, ret);
        continue;
      }

      if(ret == 0)
        r->status = CLOSED;
      else
        r->status = ERROR;
      return;
    }
  }
}
