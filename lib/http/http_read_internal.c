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
http_read_internal(fd_type fd, char* x, size_t received, buffer* b) {
  http* h = b->cookie;
  buffer* in = &h->q.in;
  http_response* response = h->response;
  ssize_t ret = received;

  switch(response->status) {
    case HTTP_RECV_HEADER: {
      while(response->status == HTTP_RECV_HEADER)
        if((ret = http_read_header(&h->q.in, &response->data, response)) <= 0)
          goto end;

      if(response->status == HTTP_RECV_DATA) {
        response->headers_len = response->data.len;

        if(!buffer_LEN(in))
          break;
      } else
        break;
    }

    case HTTP_RECV_DATA: {
      http_response_read(in, response);

      if(response->chunk_length > response->data_pos) {
        size_t avail = buffer_LEN(in), remain = response->chunk_length - response->data_pos;
        size_t num = MIN(avail, MIN(remain, received));

        byte_copy(x, num, buffer_PEEK(in));
        buffer_SKIP(in, num);

        response->data_pos += num;
        ret = num;
      } else if(response->chunk_length == response->data_pos) {
        response->chunk_length = 0;
      }

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
  buffer_putspad(buffer_2, "\033[1;33mhttp_read_internal\033[0m ", 30);

  buffer_puts(buffer_2, " ret=");
  buffer_putlong(buffer_2, ret);

  buffer_puts(buffer_2, " headers_len=");
  buffer_putulonglong(buffer_2, response->headers_len);
  buffer_puts(buffer_2, " chunk_length=");
  buffer_putulonglong(buffer_2, response->chunk_length);

  buffer_puts(buffer_2, " content_length=");
  buffer_putulonglong(buffer_2, response->content_length);

  if(ret < 0) {
    buffer_puts(buffer_2, " err=");
    buffer_putstr(buffer_2, http_strerror(h, received));
  }

  if(h->response->code != -1) {
    buffer_puts(buffer_2, " code=");
    buffer_putlong(buffer_2, h->response->code);
  }

  buffer_putnlflush(buffer_2);
#endif

  return ret;
}
