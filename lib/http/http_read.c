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
http_read(http* h, char* buf, size_t len, buffer* bf) {
  ssize_t bytes, n, ret = 0;
  http_response* r;
  buffer* b;
  h = bf->cookie;
  b = &h->q.in;
  r = h->response;
  while(len) {
    int st = r->status;
    bytes = b->n - b->p;
    if((n = buffer_freshen(b)) <= 0) {
      if((int)r->status == st) {
        if(ret == 0 && r->err != 0) {
          errno = r->err;
          ret = -1;
        }
        break;
      }
    }
    if(b->n - b->p > (unsigned long)bytes)
      // putnum("growbuf", (b->n - b->p) - bytes);
      // buffer_dump(buffer_2, b);
      if(h->response->status != HTTP_RECV_DATA)
        break;
    if(n + r->ptr > r->content_length)
      n = r->content_length - r->ptr;
    if(n >= (ssize_t)len)
      n = (ssize_t)len;
    byte_copy(buf, (size_t)n, b->x + b->p);
    // putnum("skipbuf", n);
    len -= (size_t)n;
    buf += n;
    ret += n;
    b->p += (size_t)n;
    if(b->p == b->n)
      b->p = b->n = 0;
    // r->content_length -= n;
    r->ptr += n;
    // putnum("ptr", r->ptr);
    if(r->ptr == r->content_length && b->n - b->p > 0) {
      http_read_internal(h, &b->x[b->p], b->n - b->p);

      if(r->status == HTTP_STATUS_FINISH) {
        io_dontwantread(h->sock);
        break;
      }
    }
  }
  // putnum("ret", ret);
  // putnum("avail", b->n - b->p);
  return ret;
}
