#include "../http.h"
#include "../scan.h"
#include "../socket_internal.h"
#include "../str.h"
#include "../stralloc.h"
#include "../io.h"
#include "../byte.h"
#include <errno.h>
#include <assert.h>

ssize_t http_socket_read(fd_t fd, void* buf, size_t len, void* b);

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
    while(l-- > 0) buffer_put(buffer_2, b++, 1);
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
      r->ptr = in->p;
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

    if(str_start(x, "Content-Type: multipart")) {
      size_t p = str_find(x, "boundary=");
      if(x[p]) {
        stralloc_copys(&r->boundary, &x[p + str_len("boundary=")]);
      }
      r->transfer = HTTP_TRANSFER_BOUNDARY;
    } else if(str_start(x, "Content-Length:")) {
      scan_ulonglong(&sa->s[16], &r->content_length);
      r->transfer = HTTP_TRANSFER_LENGTH;
    } else if(str_start(x, "Transfer-Encoding:") && str_contains(x, "chunked")) {
      r->chunk_length = 0;
      r->transfer = HTTP_TRANSFER_CHUNKED;
    }
  }
  h->q.in.op = (buffer_op_proto*)&http_socket_read;
  return ret;
}
