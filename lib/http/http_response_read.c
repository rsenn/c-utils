#include "../http.h"
#include "../scan.h"
#include "../byte.h"
#include "../util.h"

ssize_t
http_response_read(buffer* in, http_response* response) {
  ssize_t ret = 0;

  switch(response->transfer) {
    case HTTP_TRANSFER_BOUNDARY: {
      break;
    }

    case HTTP_TRANSFER_CHUNKED: {
    again:
      if(response->chunk_length == 0) {
        size_t i;
        size_t n = buffer_LEN(in);
        char* x = buffer_PEEK(in);

        if((i = byte_chrs(x, n, "\r\n", 2)) < n) {
          i = scan_xlonglong(x, &response->chunk_length);

          buffer_SKIP(in, i);
          buffer_SCAN(in, scan_eolskip);
          // response->headers_len = 0;

          if(response->chunk_length)
            response->content_length += response->chunk_length;
          else
            response->status = HTTP_STATUS_FINISH;

#ifdef DEBUG_HTTP
          buffer_putspad(buffer_2, "\033[1;36mparsed chunk_length\033[0m", 30);
          buffer_puts(buffer_2, "chunk_len='");
          buffer_put(buffer_2, x, i);
          buffer_puts(buffer_2, "' i=");
          buffer_putlong(buffer_2, i);
          buffer_puts(buffer_2, " response->headers_len=");
          buffer_putulonglong(buffer_2, response->headers_len);
          buffer_puts(buffer_2, " response->chunk_length=");
          buffer_putulonglong(buffer_2, response->chunk_length);
          buffer_puts(buffer_2, " response->content_length=");
          buffer_putulonglong(buffer_2, response->content_length);
          buffer_puts(buffer_2, " response->data_pos=");
          buffer_putulonglong(buffer_2, response->data_pos);
          buffer_putnlflush(buffer_2);
#endif
        }
      } else if(response->chunk_length == response->data_pos) {
        response->data_pos = 0;
        response->chunk_length = 0;

        buffer_SCAN(in, scan_eolskip);

        /*        if(buffer_LEN(in))
                  goto again;*/

      } else {
        ssize_t n = buffer_LEN(in);
        ssize_t remain = response->chunk_length - response->data_pos;

        ret = MIN(remain, n);
      }

      break;
    }

    case HTTP_TRANSFER_LENGTH: {
      if(response->chunk_length >= response->content_length)
        response->status = HTTP_STATUS_FINISH;

      break;
    }
  }

  return ret;
}
