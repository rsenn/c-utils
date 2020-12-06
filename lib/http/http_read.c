#include "../http.h"
#include "../scan.h"
#include "../socket_internal.h"
#include "../str.h"
#include "../stralloc.h"
#include "../io.h"
#include "../byte.h"
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
  ssize_t bytes, n, ret = 0;
  http_response* r;
  http* h = (http*)((buffer*)ptr)->cookie;
  buffer* b = &h->q.in;
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
  return ret;
}
