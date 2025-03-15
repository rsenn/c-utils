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
  // int status = r->status;

  switch(r->status) {
    case HTTP_RECV_HEADER: {
      while(r->status == HTTP_RECV_HEADER) {
        ssize_t ret;

        if((ret = http_read_header(&h->q.in, &r->data, r)) <= 0)
          break;
      }

      if(r->status == HTTP_RECV_DATA)
        r->headers_len = r->data.len;

      goto end;
    }

    case HTTP_RECV_DATA: {
      size_t len = r->content_length;

      if(len > r->data_pos) {
        size_t remain = len - r->data_pos;
        size_t num = MIN(received, remain);

        byte_copy(buf, num, buffer_PEEK(in));

        buffer_SKIP(in, num);

        r->data_pos += num;
        n = num;
      }

      switch(r->transfer) {
        case HTTP_TRANSFER_BOUNDARY: break;

        case HTTP_TRANSFER_CHUNKED: {
          /* if(r->chunk_length >= r->content_length) {
             size_t skip;

             if((skip = scan_eolskip(buffer_PEEK(in), buffer_LEN(in)))) {
               buffer_SKIP(in, skip);
               r->chunk_length = 0;
             }
           }*/

          if(r->chunk_length == 0) {
            size_t i;

            if((i = byte_chr(buffer_PEEK(in), buffer_LEN(in), '\n')) < buffer_LEN(in)) {
              i = scan_xlonglong(buffer_PEEK(in), &r->chunk_length);

              buffer_SKIP(in, i);

              if((i = scan_eolskip(buffer_PEEK(in), buffer_LEN(in))))
                buffer_SKIP(in, i);

              // r->headers_len = 0;

              if(r->chunk_length)
                r->content_length += r->chunk_length;
              else
                r->status = HTTP_STATUS_FINISH;

#ifdef DEBUG_HTTP
              buffer_putspad(buffer_2, "\033[1;36mparsed chunk_length\033[0m ", 30);
              buffer_puts(buffer_2, "i=");
              buffer_putlong(buffer_2, i);
              buffer_puts(buffer_2, " r->headers_len=");
              buffer_putulonglong(buffer_2, r->headers_len);
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

      break;
    }
    case HTTP_STATUS_CLOSED: {
      io_dontwantread(h->sock);
      io_dontwantwrite(h->sock);
      n = 0;
      break;
    }

    case HTTP_STATUS_ERROR: {
      n = -1;
      break;
    }

    case HTTP_STATUS_BUSY:
    case HTTP_STATUS_FINISH: {
      break;
    }
  }

end:

#ifdef DEBUG_HTTP
  buffer_putspad(buffer_2, "\033[1;33mhttp_read_internal\033[0m ", 30);

  buffer_puts(buffer_2, " ret=");
  buffer_putlong(buffer_2, n);

  buffer_puts(buffer_2, " headers_len=");
  buffer_putulonglong(buffer_2, r->headers_len);
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

#endif

  return n;
}
