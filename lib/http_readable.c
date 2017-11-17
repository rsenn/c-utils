#include "http.h"
#include "buffer.h"
#include <errno.h>
#include <sys/socket.h>

ssize_t
buffer_dummyread() {
  return 0;
}

void
putline(const char* what, const char*b, size_t l) {
  buffer_puts(buffer_2, what);
  buffer_puts(buffer_2, "[");
  buffer_putulong(buffer_2, l);
  buffer_puts(buffer_2, "]");
  buffer_puts(buffer_2, ": ");
  buffer_put(buffer_2, b, l);
  buffer_putnlflush(buffer_2);
}

void
http_readable(http* h) {
  if(h->response) {
    char recvbuf[8192];
    ssize_t  ret;
    http_response* r = h->response;
    buffer recvb  = BUFFER_INIT(buffer_dummyread, -1, r->body.s, r->body.len);
    recvb.p = r->ptr;

    for(;;) {
      ret = recv(h->sock, recvbuf, sizeof(recvbuf), 0);

      if(ret == -1 && errno == EAGAIN) {
        errno = 0;
        break;
      }

      if(ret > 0) {
        stralloc_catb(&r->body, recvbuf, ret);
        recvb.a = recvb.n = r->body.len;
        recvb.x = r->body.s;
      }

      if(ret == 0)
        r->status = CLOSED;
      else
        r->status = ERROR;
      break;

    }

    for(;;)   {
      char line[1024];
      int ret = buffer_getline(&recvb, line, sizeof(line));

      r->ptr = recvb.p;

      if(ret >= 0) {

        while(ret > 0 && isspace(line[ret - 1]))
          ret--;

        unsigned long n;
        if(scan_ulong(line, &n) > 0) {

          if(r->part == HEADER)
            r->part = CHUNKS;


          if(recvb.n - recvb.p >= n) {
            stralloc_readyplus(&r->data, n);
            buffer_get(&recvb, &r->data.s[r->data.len], n);
            continue;
          } else {
            return;
          }
        }
        if(!r->part) {
          putline("Response", line, ret);

          r->part = HEADER;

        } else if(r->part == HEADER) {
          putline("Header", line, ret);

        } else if(r->part = CHUNKS) {
          if(ret > 0)
            putline("Chunks", line, ret);

          else
            break;
        }
      }
    }
  }
}
