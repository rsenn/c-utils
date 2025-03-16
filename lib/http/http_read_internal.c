#include "../http.h"
#include "../io.h"
#include "../byte.h"
#include "../fmt.h"
#include "../util.h"

/**
 * @brief http_read_internal
 * @param h       http* struct
 * @param buf     buffer we've just written to
 * @param received     number of bytes we've just written
 * @return
 */
ssize_t
http_read_internal(fd_type fd, char* y, size_t l, buffer* b) {
  http* h = b->cookie;
  buffer* in = &h->q.in;
  http_response* response = h->response;
  ssize_t ret = 0;
  char* x = y;
  size_t n = l;
  uint32_t iteration = 0;

again:
  ++iteration;

  switch(response->status) {
    case HTTP_RECV_HEADER: {
      while(response->status == HTTP_RECV_HEADER) {
        ssize_t r = http_read_header(in, &response->data, response);

        if(r <= 0)
          goto end;
      }

      if(response->status == HTTP_RECV_DATA) {
        response->headers_len = response->data.len;

        if(buffer_LEN(in))
          goto again;
      }

      break;
    }

    case HTTP_RECV_DATA: {
      ssize_t r = http_response_read(in, response);

      if(r > 0) {
        r = MIN(r, n);

        byte_copy(x, r, buffer_PEEK(in));
        buffer_SKIP(in, r);

        response->data_pos += r;
        ret += r;

        x += r;
        n -= r;
      }

      if(r >= 0)
        if(n > 0)
          if(buffer_LEN(in))
            goto again;

      break;
    }

    case HTTP_STATUS_CLOSED: {
      io_dontwantread(h->sock);
      io_dontwantwrite(h->sock);
      ret = 0;
      break;
    }

    case HTTP_STATUS_ERROR: {
      ret = -1;
      break;
    }

    case HTTP_STATUS_BUSY:
    case HTTP_STATUS_FINISH: {
      break;
    }
  }

end:

#ifdef DEBUG_HTTP
  buffer_putspad(buffer_2, "\033[1;33mhttp_read_internal\033[0m", 30);

  buffer_puts(buffer_2, "headers_len=");
  buffer_putulonglong(buffer_2, response->headers_len);
  buffer_puts(buffer_2, " chunk_length=");
  buffer_putulonglong(buffer_2, response->chunk_length);

  buffer_puts(buffer_2, " content_length=");
  buffer_putulonglong(buffer_2, response->content_length);

  buffer_puts(buffer_2, " data_pos=");
  buffer_putulonglong(buffer_2, response->data_pos);

  if(ret < 0) {
    buffer_puts(buffer_2, " err=");
    buffer_putstr(buffer_2, http_strerror(h, ret));
  }

  if(h->response->code != -1) {
    buffer_puts(buffer_2, " code=");
    buffer_putlong(buffer_2, h->response->code);
  }

  buffer_puts(buffer_2, " ret=");
  buffer_putlong(buffer_2, ret);

  buffer_putnlflush(buffer_2);
#endif

  return ret;
}
