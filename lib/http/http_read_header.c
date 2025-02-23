#include "../http.h"
#include "../scan.h"
#include "../socket_internal.h"
#include "../str.h"
#include "../stralloc.h"
#include "../io.h"
#include "../byte.h"
#include "../case.h"
#include <errno.h>
#include <assert.h>

ssize_t http_socket_read(fd_type fd, void* buf, size_t len, void* b);

static inline void
putline(const char* what, const char* b, ssize_t l, int i) {
  buffer_puts(buffer_2, what);
  buffer_puts(buffer_2, "[");
  buffer_putulong(buffer_2, i);
  buffer_puts(buffer_2, "]");
  buffer_puts(buffer_2, ": ");
  if(l <= 0)
    buffer_puts(buffer_2, b);
  else {
    while(l-- > 0)
      buffer_put(buffer_2, b++, 1);
  }
  buffer_putnlflush(buffer_2);
}

ssize_t
http_read_header(http* h, stralloc* sa, http_response* r) {
  ssize_t ret = 0, bytesread = 0;
  size_t start, n;
  char* x;
  buffer* in = &h->q.in;

  while(r->status == HTTP_RECV_HEADER) {
    size_t bytesavail = in->n - in->p;
    // h->q.in.op = NULL;
    start = sa->len;
    if((ret = buffer_getline_sa(&h->q.in, sa)) <= 0)
      break;

    bytesread += bytesavail - (in->n - in->p);
    stralloc_nul(sa);
    x = &sa->s[start];
    n = byte_trimr(x, sa->len - start, "\r\n", 2);
    if(n == 0) {
      // r->ptr = in->p;
      r->status = HTTP_RECV_DATA;
      ret = 1;
      break;
    }
#if DEBUG_HTTP
    putline("Header", x, n, byte_count(sa->s, sa->len, '\n'));
#endif
    if(r->code == -1) {

      if(str_start(sa->s, "HTTP")) {
        unsigned int code;
        size_t p = scan_nonwhitenskip(sa->s, sa->len);
        p += scan_whitenskip(&sa->s[p], sa->len - p);
        if(scan_uint(&sa->s[p], &code) > 0)
          r->code = code;
      }
    }

    if(sa->len - start >= 23 && !case_diffb(x, 23, "Content-Type: multipart")) {
      size_t p = str_find(x, "boundary=");
      if(x[p]) {
        stralloc_copys(&r->boundary, &x[p + str_len("boundary=")]);
      }
      r->transfer = HTTP_TRANSFER_BOUNDARY;
      r->ptr = 0;
    } else if(sa->len - start >= 15 && !case_diffb(x, 15, "Content-Length:")) {
      scan_ulonglong(&x[16], &r->content_length);
      r->transfer = HTTP_TRANSFER_LENGTH;
      r->ptr = 0;
    } else if(sa->len - start >= 18 && case_diffb(x, 18, "Transfer-Encoding:") && str_contains(x, "chunked")) {
      r->ptr = 0;
      r->chunk_length = 0;
      r->content_length = 0;
      r->transfer = HTTP_TRANSFER_CHUNKED;
    }
  }
  // h->q.in.op = (buffer_op_proto*)&http_socket_read;

#ifdef DEBUG_HTTP
  buffer_putspad(buffer_2, "\x1b[1;33mhttp_read_header\x1b[0m", 30);
  buffer_puts(buffer_2, "s=");
  buffer_putlong(buffer_2, h->sock);

  buffer_puts(buffer_2, " bytesread=");
  buffer_putlong(buffer_2, bytesread);
  buffer_puts(buffer_2, " ret=");
  buffer_putlong(buffer_2, ret);
  if(ret < 0) {
    buffer_puts(buffer_2, " err=");
    buffer_putstr(buffer_2, http_strerror(h, ret));
  }

  if(r->code != -1) {
    buffer_puts(buffer_2, " code=\x1b[1;36m");
    buffer_putlong(buffer_2, r->code);
    buffer_puts(buffer_2, "\x1b[0m");
  }
  if(r->content_length > 0) {
    buffer_puts(buffer_2, " content_length=");
    buffer_putlong(buffer_2, r->content_length);
  }
  buffer_puts(buffer_2, " status=");
  buffer_puts(
      buffer_2,
      ((const char* const[]){"-1", "HTTP_RECV_HEADER", "HTTP_RECV_DATA", "HTTP_STATUS_CLOSED", "HTTP_STATUS_ERROR", "HTTP_STATUS_BUSY", "HTTP_STATUS_FINISH", 0})[r->status + 1]);
  buffer_putnlflush(buffer_2);
#endif
  return ret > 0 && bytesread > 0 ? bytesread : ret;
}
