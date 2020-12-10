#include "../http.h"
#include "../scan.h"
#include "../socket_internal.h"
#include "../str.h"
#include "../stralloc.h"
#include "../io.h"
#include "../byte.h"
#include "../fmt.h"
#include <errno.h>
#include <assert.h>

/**
 * @brief http_read_internal
 * @param h       http* struct
 * @param buf     buffer we've just written to
 * @param received     number of bytes we've just written
 * @return
 */
size_t
http_read_internal(fd_t fd, char* buf, size_t received, buffer* b) {
  http* h = b->cookie;
  buffer* in = &h->q.in;
  char* x = buffer_PEEK(in);
  char* y = buf + received;
  http_response* r = h->response;
  ssize_t n = received;
  int status = r->status;

  if((r = h->response)) {

    while(r->status == HTTP_RECV_HEADER && http_read_header(h, &r->data, r) > 0) {
      r->ptr = 0;
    }
    if(r->status == HTTP_RECV_DATA) {
      switch(r->transfer) {
        case HTTP_TRANSFER_UNDEF: break;
        case HTTP_TRANSFER_BOUNDARY: break;
        case HTTP_TRANSFER_CHUNKED: {
          if(r->ptr == r->chunk_length) {
            size_t skip;
            if((skip = scan_eolskip(&in->x[in->p], in->n - in->p))) {
              in->p += skip;
              r->chunk_length = 0;
            }
          }
          if(r->chunk_length == 0) {
            size_t i, bytes = in->n - in->p;
            if((i = byte_chr(&in->x[in->p], bytes, '\n')) < bytes) {
              i = scan_xlonglong(&in->x[in->p], &r->chunk_length);

#ifdef DEBUG_HTTP
              buffer_putspad(buffer_2, "parsed chunk_length ", 30);
              buffer_puts(buffer_2, "i=");
              buffer_putlong(buffer_2, i);
              buffer_puts(buffer_2, " r->chunk_length=");
              buffer_puts(buffer_2, "\x1b[1;36m");
              buffer_putulonglong(buffer_2, r->chunk_length);

              buffer_puts(buffer_2, "\x1b[0m");
              buffer_putnlflush(buffer_2);
#endif
              in->p += i;
              if((i = scan_eolskip(&in->x[in->p], in->n - in->p)))
                in->p += i;
              r->ptr = 0;
              if(r->chunk_length) {
              } else {
                r->status = HTTP_STATUS_FINISH;
              }
            }
          }
          break;
        }
        case HTTP_TRANSFER_LENGTH: {
          if(r->ptr == r->content_length)
            r->status = HTTP_STATUS_FINISH;
          break;
        }
      }
    }
    if(r->status == HTTP_STATUS_ERROR) {
      n = -1;
    } else if(r->status == HTTP_STATUS_CLOSED) {
      io_dontwantread(h->sock);
      io_dontwantwrite(h->sock);
      n = 0;
    } else {
      n = received;
    }
  }
#ifdef DEBUG_HTTP
  buffer_putspad(buffer_2, "http_read_internal ", 30);
  buffer_puts(buffer_2, "s=");
  buffer_putlong(buffer_2, h->sock);
  buffer_puts(buffer_2, " ret=");
  buffer_putlong(buffer_2, n);

  if(n < 0) {
    buffer_puts(buffer_2, " err=");
    buffer_putstr(buffer_2, http_strerror(h, received));
  }
  if(h->response->code != -1) {
    buffer_puts(buffer_2, " code=");
    buffer_putlong(buffer_2, h->response->code);
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
                                     0})[status + 1]);
  if(received > 0) {
    size_t len = received;
    const char* s = stralloc_end(&r->data) - len;
    const char* e = stralloc_end(&r->data);
    if(len > 30)
      len = 30;
    buffer_puts(buffer_2, "\n  received=");
    buffer_putlong(buffer_2, received);
    buffer_puts(buffer_2, " len=");
    buffer_putlong(buffer_2, r->data.len);

    buffer_puts(buffer_2, " x=");

    buffer_put_escaped(buffer_2, stralloc_end(&r->data) - received, len, &fmt_escapecharshell);

    if(len < received) {
      buffer_puts(buffer_2, " ... more (");
      buffer_putulong(buffer_2, received);
      buffer_puts(buffer_2, " bytes total) ...");
    }
  }
  buffer_putnlflush(buffer_2);
#endif
  return n;
}
