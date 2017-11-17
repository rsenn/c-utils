#include "http.h"
#include "buffer.h"
#include <errno.h>
#include <sys/socket.h>

ssize_t
buffer_dummyread() {
  return 0;
}

void
putline(const char* what, const char*b, ssize_t l, buffer *buf) {
  buffer_puts(buffer_2, what);
  buffer_puts(buffer_2, "[");
  buffer_putulong(buffer_2, l <= 0 ? -l : l);
  buffer_puts(buffer_2, "]");
  buffer_puts(buffer_2, ": ");
  if(l <= 0)
    buffer_puts(buffer_2, b);
  else
    buffer_put(buffer_2, b, l);
  buffer_puts(buffer_2, " (bytes in recvb: ");
  buffer_putulong(buffer_2, buf->n - buf->p);
  buffer_puts(buffer_2, ")");
  buffer_putnlflush(buffer_2);
}

void
http_readable(http* h) {
  if(h->response) {
    char recvbuf[8192];
    ssize_t  ret;
    int err;
    http_response* r = h->response;
    buffer recvb  = BUFFER_INIT(buffer_dummyread, -1, r->body.s, r->body.len);
    recvb.p = r->ptr;
again:
    for(;;) {
      ret = recv(h->sock, recvbuf, sizeof(recvbuf), 0);
      err = errno;

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
      else if(err != 0)
        r->status = ERROR;
      break;

    }

    for(;;)   {
      char line[1024];
      size_t sptr = r->ptr;
      int ret = buffer_getline(&recvb, line, sizeof(line));

      r->ptr = recvb.p;

      if(ret >= 0) {

        while(ret > 0 && isspace(line[ret - 1]))
          ret--;

        line[ret] = '\0';

        unsigned long n, p;

        if(r->part < CHUNKS && line[str_chr(line, ':')] == ':') {
          if(r->part == HEADER)
            putline("Header", line, ret, &recvb);
          r->part = HEADER;

        } else {
          if(r->part == HEADER)
            r->part = CHUNKS;
        }

        if(r->part == CHUNKS && (p = scan_xlong(line, &n)) > 0) {

          putline("Chunk", line, -r->chnk, &recvb);

          if(n == 0) {
            r->status = DONE;
            return;
          } else if(recvb.n - recvb.p >= n) {
            stralloc_readyplus(&r->data, n);
            buffer_get(&recvb, &r->data.s[r->data.len], n);
            r->data.len += n;


            buffer_puts(buffer_1, "data len=");
            buffer_putulong(buffer_1, r->data.len);
            buffer_putnlflush(buffer_1);


            r->chnk++;

          } else {
            r->ptr = sptr;
            return; //goto again;
          }

          ssize_t n = buffer_getline(&recvb, line, sizeof(line));
          putline("Newline", "", n, &recvb);
          if(recvb.n - recvb.p <= 0)
            return;

          continue;
        }
        if(!r->part) {
          putline("Response", line, ret, &recvb);
        }
      }
    }
  }
}
