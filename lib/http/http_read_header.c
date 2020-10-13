#include "../http.h"
#include "../scan.h"
#include "../socket_internal.h"
#include "../str.h"
#include "../stralloc.h"
#include "../io.h"
#include "../byte.h"
#include <errno.h>
#include <assert.h>

static inline void
putline(const char* what, const char* b, ssize_t l,  int i) {
  buffer_puts(buffer_2, what);
  buffer_puts(buffer_2, "[");
  buffer_putulong(buffer_2, i); //l <= 0 ? -l : l);
  buffer_puts(buffer_2, "]");
  buffer_puts(buffer_2, ": ");
  if(l <= 0)
    buffer_puts(buffer_2, b);
  else {
    while(l-- > 0) buffer_put(buffer_2, b++, 1);
  }
  /*
  buffer_puts(buffer_2, " (bytes in recvb: ");
  buffer_putulong(buffer_2, buf->n - buf->p);
  buffer_puts(buffer_2, ")");*/
  buffer_putnlflush(buffer_2);
}

ssize_t
http_read_header(http* h, stralloc* sa, http_response* r) {
  ssize_t ret = 0, bytesread = 0;
  size_t start, n;
  const char* x;
  buffer* in = &h->q.in;
  while(r->status == HTTP_RECV_HEADER) {
    size_t bytesavail = in->n - in->p;
    h->q.in.op = NULL;
    start = sa->len;

    if((ret = buffer_getline_sa(&h->q.in, sa)) <= 0)
      break;
    bytesread += bytesavail - (in->n - in->p);
    //  stralloc_trimr(sa, "\r\n", 2);
    stralloc_nul(sa);

    x = &sa->s[start];
    n = byte_trimr(x, sa->len - start, "\r\n", 2);
    if(n == 0) {
      r->ptr = in->p;
      r->status = HTTP_RECV_DATA;
      ret = 1;
      break;
    }

#if 1  || DEBUG_HTTP
    putline("Header", x, n, byte_count(sa->s, sa->len, '\n'));
#endif
 /*   buffer_putm_internal(buffer_2, "x:", x, 0);
    buffer_puts(buffer_2, "n:");
    buffer_putulong(buffer_2, n);
    //  buffer_putm_internal(buffer_2, "\nsa:", sa->s, 0);
    buffer_putnlflush(buffer_2);*/

    if(str_start(x, "Content-Type: multipart")) {
      size_t p = str_find(x, "boundary=");
      if(x[p]) {
        stralloc_copys(&r->boundary, &x[p + str_len("boundary=")]);
      }
      r->transfer = HTTP_TRANSFER_BOUNDARY;
    } else if(str_start(x, "Content-Length:")) {
      scan_ulonglong(&sa->s[16], &r->content_length);
      r->transfer = HTTP_TRANSFER_LENGTH;
      // putnum("content length", r->content_length);
    } else if(str_start(x, "Transfer-Encoding:") && str_contains(x, "chunked")) {
      r->chunk_length = 0;
      r->transfer = HTTP_TRANSFER_CHUNKED;
    }
  }
  // putnum("http_read_header", r->status);
  h->q.in.op = (buffer_op_proto*)&http_socket_read;
  return ret;
}