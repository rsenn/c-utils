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

#ifndef MIN
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#endif

/**
 * @brief http_read_internal
 * @param h       http* struct
 * @param buf     buffer we've just written to
 * @param received     number of bytes we've just written
 * @return
 */
ssize_t
http_read_internal(fd_type fd, char* buf, size_t received, buffer* b) {
  http* h = b->cookie;
  buffer* in = &h->q.in;
  http_response* r = h->response;
  ssize_t n = received;
  int status = r->status;

#ifdef DEBUG_HTTP
  buffer_putspad(buffer_2, "\033[1;33mhttp_read_internal\033[0m ", 30);
  buffer_puts(buffer_2, "s=");
  buffer_putlong(buffer_2, h->sock);
  buffer_puts(buffer_2, " ret=");
  buffer_putlong(buffer_2, n);

  buffer_puts(buffer_2, " ptr=");
  buffer_putulonglong(buffer_2, r->ptr);
  buffer_puts(buffer_2, " chunk_length=");
  buffer_putulonglong(buffer_2, r->chunk_length);

  buffer_puts(buffer_2, " content_length=");
  buffer_putulonglong(buffer_2, r->content_length);

  if(n < 0) {
    buffer_puts(buffer_2, " err=");
    buffer_putstr(buffer_2, http_strerror(h, received));
  }

  if(h->response->code != -1) {
    buffer_puts(buffer_2, " code=");
    buffer_putlong(buffer_2, h->response->code);
  }

  buffer_puts(buffer_2, " transfer=");
  buffer_puts(buffer_2, "HTTP_TRANSFER_");
  buffer_puts(buffer_2,
              ((const char* const[]){
                  "UNDEF",
                  "CHUNKED",
                  "LENGTH",
                  "BOUNDARY",
                  0,
              })[r->transfer]);
  buffer_puts(buffer_2, " status=");
  buffer_puts(buffer_2,
              ((const char* const[]){
                  "-1",
                  "HTTP_RECV_HEADER",
                  "HTTP_RECV_DATA",
                  "HTTP_STATUS_CLOSED",
                  "HTTP_STATUS_ERROR",
                  "HTTP_STATUS_BUSY",
                  "HTTP_STATUS_FINISH",
                  0,
              })[status + 1]);
  buffer_putnlflush(buffer_2);

#endif

  if(r->status == HTTP_RECV_DATA) {
    if(r->chunk_length < r->content_length) {
      const char* s = buffer_PEEK(in);
      size_t remain = r->content_length - r->chunk_length;
      size_t num = MIN(received, remain);

      byte_copy(buf, num, s);

      in->p += num;

      /* if(r->data.len - r->ptr >= r->content_length)*/
      r->chunk_length += num;
      n = num;
    }
  }

  if(r->status == HTTP_RECV_HEADER) {
    while(r->status == HTTP_RECV_HEADER && http_read_header(h, &r->data, r) > 0) {
    }

    if(r->status == HTTP_RECV_DATA)
      r->ptr = r->data.len;

    return n;
  }

  if(r->status == HTTP_RECV_DATA) {
    switch(r->transfer) {
      case HTTP_TRANSFER_UNDEF: break;
      case HTTP_TRANSFER_BOUNDARY: break;
      case HTTP_TRANSFER_CHUNKED: {
        if(r->chunk_length >= r->content_length) {
          size_t skip;

          if((skip = scan_eolskip(&in->x[in->p], in->n - in->p))) {
            buffer_skipn(in, skip);
            r->chunk_length = 0;
          }
        }

        if(r->chunk_length == 0) {
          size_t i, bytes = in->n - in->p;

          if((i = byte_chr(&in->x[in->p], bytes, '\n')) < bytes) {
            i = scan_xlonglong(&in->x[in->p], &r->chunk_length);

            buffer_skipn(in, i);

            if((i = scan_eolskip(&in->x[in->p], in->n - in->p)))
              buffer_skipn(in, i);

            // r->ptr = 0;

            if(r->chunk_length)
              r->content_length += r->chunk_length;
            else
              r->status = HTTP_STATUS_FINISH;

#ifdef DEBUG_HTTP
            buffer_putspad(buffer_2, "\033[1;36mparsed chunk_length\033[0m ", 30);
            buffer_puts(buffer_2, "i=");
            buffer_putlong(buffer_2, i);
            buffer_puts(buffer_2, " r->ptr=");
            buffer_putulonglong(buffer_2, r->ptr);
            buffer_puts(buffer_2, " r->chunk_length=");
            buffer_putulonglong(buffer_2, r->chunk_length);
            buffer_puts(buffer_2, " r->content_length=");
            buffer_putulonglong(buffer_2, r->content_length);
            buffer_putnlflush(buffer_2);
#endif
          }
        }

        break;
      }

      case HTTP_TRANSFER_LENGTH: {
        if(r->chunk_length >= r->content_length)
          r->status = HTTP_STATUS_FINISH;

        break;
      }
    }
  }

  switch(r->status) {
    case HTTP_STATUS_ERROR: {
      n = -1;
      break;
    }

    case HTTP_STATUS_CLOSED: {
      io_dontwantread(h->sock);
      io_dontwantwrite(h->sock);
      n = 0;
      break;
    }

    default: {
      n = received;
      break;
    }
  }

  return n;
}
