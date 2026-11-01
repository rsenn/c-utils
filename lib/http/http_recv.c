#include "../http.h"
#include "../buffer.h"
#include "../byte.h"
#include "../scan.h"
#include <errno.h>

#ifndef EPROTO
#define EPROTO EINVAL
#endif

enum { SIZE = 0, DATA, EOL, TRAILER, END };

/* the end of the next line in the input, 0 if it is not complete yet */
static size_t
line_end(buffer* in) {
  size_t n = buffer_LEN(in), i = byte_chr(buffer_PEEK(in), n, '\n');

  return i < n ? i + 1 : 0;
}

ssize_t
http_recv(http* h, char* buf, size_t len) {
  http_response* r = h->response;
  buffer* in = &h->q.in;
  size_t got = 0;

  if(r == NULL || r->status == HTTP_RECV_HEADER || r->status == HTTP_STATUS_FINISH || r->status == 0)
    return 0;

  if(r->status == HTTP_STATUS_ERROR) {
    errno = r->err ? r->err : EIO;
    return -1;
  }

  while(got < len && r->status != HTTP_STATUS_FINISH) {
    size_t avail = buffer_LEN(in), n;

    if(r->transfer == HTTP_TRANSFER_CHUNKED) {
      if(r->chunk_state == SIZE) {
        uint64 size = 0;

        if(!(n = line_end(in)))
          break;

        if(!scan_x64(buffer_PEEK(in), &size)) {
          errno = EPROTO;
          return -1;
        }
        buffer_SKIP(in, n);

        /* the last chunk is followed by trailers and an empty line */
        r->chunk_length = size;
        r->chunk_state = size ? DATA : TRAILER;
      } else if(r->chunk_state == DATA) {
        if(!avail)
          break;

        n = avail;
        if(n > len - got)
          n = len - got;
        if(n > r->chunk_length)
          n = (size_t)r->chunk_length;

        byte_copy(buf + got, n, buffer_PEEK(in));
        buffer_SKIP(in, n);
        got += n;
        r->data_pos += n;
        r->chunk_length -= n;

        if(r->chunk_length == 0)
          r->chunk_state = EOL;
      } else if(r->chunk_state == EOL) {
        if(!(n = line_end(in)))
          break;
        buffer_SKIP(in, n);
        r->chunk_state = SIZE;
      } else if(r->chunk_state == TRAILER) {
        if(!(n = line_end(in)))
          break;
        /* an empty line has only the line end */
        if(n <= 2 && (n == 1 || buffer_PEEK(in)[0] == '\r')) {
          r->chunk_state = END;
          r->status = HTTP_STATUS_FINISH;
        }
        buffer_SKIP(in, n);
      }
    } else {
      if(!avail)
        break;

      n = avail;
      if(n > len - got)
        n = len - got;

      if(r->transfer == HTTP_TRANSFER_LENGTH) {
        uint64 remain = r->content_length - r->data_pos;

        if(n > remain)
          n = (size_t)remain;
      }

      byte_copy(buf + got, n, buffer_PEEK(in));
      buffer_SKIP(in, n);
      got += n;
      r->data_pos += n;

      if(r->transfer == HTTP_TRANSFER_LENGTH && r->data_pos >= r->content_length)
        r->status = HTTP_STATUS_FINISH;
    }
  }

  /* a body without length ends when the connection does */
  if(r->status == HTTP_STATUS_CLOSED && buffer_LEN(in) == 0 && got == 0) {
    if(r->transfer == HTTP_TRANSFER_UNDEF || (r->transfer == HTTP_TRANSFER_LENGTH && r->data_pos >= r->content_length)) {
      r->status = HTTP_STATUS_FINISH;
    } else {
      /* cut off in the middle of the body */
      errno = EPIPE;
      return -1;
    }
  }

  return (ssize_t)got;
}
