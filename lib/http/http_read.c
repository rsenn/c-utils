#include "../http.h"
#include "../scan.h"
#include "../socket_internal.h"
#include "../str.h"
#include "../stralloc.h"
#include "../io.h"
#include "../byte.h"
#include "../buffer.h"
#include <errno.h>
#include <assert.h>

static void
putnum(const char* what, ssize_t n) {
  buffer_puts(buffer_2, what);
  buffer_puts(buffer_2, ": ");
  buffer_putlonglong(buffer_2, n);
  buffer_putnlflush(buffer_2);
}

ssize_t
http_read(fd_t fd, char* buf, size_t len, void* ptr) {
  ssize_t bytes, pos = 0, end, n, ret = 0;
  http_response* r;
  const char* location = 0;
  http* h = (http*)((buffer*)ptr)->cookie;
  buffer* b = &h->q.in;
again:
  r = h->response;
  while(len) {
    int st = r->status;
    bytes = b->n - b->p;
    if((n = buffer_freshen(b)) <= 0) {
      /* if(n == 0) {
         r->status = HTTP_STATUS_CLOSED;
         ret = n;
         break;
       } else*/
      if((int)r->status == st) {
        if(ret == 0 && r->err != 0) {
          errno = r->err;
          ret = -1;
        }
        break;
      }
    }
    if(b->n - b->p > (unsigned long)bytes)
      if(h->response->status != HTTP_RECV_DATA)
        break;
    if(n + r->ptr > r->content_length)
      n = r->content_length - r->ptr;
    if(n >= (ssize_t)len)
      n = (ssize_t)len;
    byte_copy(buf, (size_t)n, b->x + b->p);
    len -= (size_t)n;
    buf += n;
    ret += n;
    b->p += (size_t)n;
    if(b->p == b->n)
      b->p = b->n = 0;
    r->ptr += n;
    if(r->ptr == r->content_length && b->n - b->p > 0) {
      http_read_internal(h->sock, &b->x[b->p], b->n - b->p, &h->q.in);
      if(r->status == HTTP_STATUS_FINISH) {
        break;
      }
    }
  }

  if(r->status == HTTP_STATUS_FINISH || r->status == HTTP_STATUS_CLOSED) {
 

    if(r->code == 302) {
      location = http_get_header(h, "Location");
      size_t len;
      pos = 0;
      end = len = str_chrs(location, "\r\n\0", 3);

      if(pos = byte_finds(location, len, "://")) {
        pos += 3;
        len -= pos;
      }

      pos += byte_chr(&location[pos], len, '/');

      if(http_get(h, &location[pos])) {
        io_onlywantwrite(h->sock);
        errno = EAGAIN;
        ret = -1;
      }
    }
  }
#ifdef DEBUG_HTTP
  buffer_putspad(buffer_2, "http_read ", 30);
  if(h->response->code == 302) {
    buffer_puts(buffer_2, "location=");
    buffer_put(buffer_2, &location[pos], end - pos);
  }
  if(h->response->code != -1) {
    buffer_puts(buffer_2, " code=");
    buffer_putulong(buffer_2, h->response->code);
  }
  buffer_puts(buffer_2, " status=");
  buffer_puts(buffer_2,
              ((const char* const[]){"-1",
                                     "HTTP_RECV_HEADER",
                                     "HTTP_RECV_DATA",
                                     "HTTP_STATUS_CLOSED",
                                     "HTTP_STATUS_ERROR",
                                     "HTTP_STATUS_BUSY",
                                     "HTTP_STATUS_FINISH",
                                     0})[h->response->status + 1]);
  buffer_putnlflush(buffer_2);
#endif
  return ret;
}
