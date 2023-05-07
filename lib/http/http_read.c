#include "../http.h"
#include "../scan.h"
#include "../socket_internal.h"
#include "../str.h"
#include "../stralloc.h"
#include "../io.h"
#include "../byte.h"
#include "../buffer.h"
#include "../fmt.h"
#include <errno.h>
#include <string.h>
#include <assert.h>

#ifndef MIN
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#endif

static void
putnum(const char* what, ssize_t n) {
  buffer_puts(buffer_2, what);
  buffer_puts(buffer_2, ": ");
  buffer_putlonglong(buffer_2, n);
  buffer_putnlflush(buffer_2);
}

ssize_t
http_read(fd_t fd, char* buf, size_t len, void* ptr) {
  size_t bytes, received;
  size_t pos = 0, end;
  ssize_t n, ret = 0;
  http_response* r;
  const char* location = 0;
  http* h = ((buffer*)ptr)->cookie;

  buffer* b = &h->q.in;

  if((h = (http*)((buffer*)ptr)->cookie) == 0)
    h = (http*)(uintptr_t)fd;
again:

  r = h->response;
  if(len) {
    const char* x;
    int st = r->status;
    bytes = buffer_LEN(b);

    if((n = (r->status == HTTP_RECV_HEADER ? buffer_freshen(b) : buffer_feed(b))) <= 0) {
      if(n == bytes || n == 0) {
        r->status = HTTP_STATUS_CLOSED;
        if(n == 0)
          goto end;
      }

      if((int)r->status == st) {
        if(r->err != 0) {
          errno = r->err;
          ret = -1;
        }
      }
    }

    x = buffer_BEGIN(b);
    n = buffer_LEN(b);

    received = n - bytes;

    /*if(r->status == HTTP_RECV_HEADER) {
        if((ret = http_read_header(h, &r->data, r)) <= 0)
          goto end;
        ret = -1;
        errno = EAGAIN;
      }*/

    if((received > 0 || r->status == HTTP_RECV_HEADER) && (ret = http_read_internal(h->sock, buf, received, &h->q.in)) > 0) {
    }
    if(r->status == HTTP_STATUS_FINISH) {
      goto end;
    } /*else {
      errno = EAGAIN;
      ret = -1;
    }*/

    if(r->status == HTTP_RECV_DATA) {
      // n = MIN(n, r->content_length - r->chunk_length);
      n = MIN(n, len);
      byte_copy(buf, n, buffer_BEGIN(b));
      // len -= (size_t)n;
      //  buf += n;
      ret = n;
      buffer_skipn(b, n);
      r->ptr += n;
    }
    if((r->status == HTTP_STATUS_CLOSED) || r->status == HTTP_STATUS_FINISH)
      goto end;
  }
end:

  /*if(r->status == HTTP_STATUS_FINISH || r->status == HTTP_STATUS_CLOSED) */ {}

  if(r->code == 302) {
    location = http_get_header(h, "Location");
    size_t len;
    pos = 0;
    end = len = str_chrs(location, "\r\n\0", 3);
    if(pos = byte_finds(location, len, "://")) {
      pos += 3;
      len -= pos;
    }
    pos += byte_chr(&location[pos], len, '/');
    if(http_get(h, &location[pos])) {
      io_onlywantwrite(h->sock);
      errno = EAGAIN;
      ret = -1;
    }
  }
  if(r->status == HTTP_STATUS_CLOSED) {
    http_close(h);
    // ret = 0;
  }

#ifdef DEBUG_HTTP
  if(r->status == HTTP_STATUS_BUSY || r->status == HTTP_RECV_HEADER || r->status == HTTP_RECV_DATA) {
    buffer_putspad(buffer_2, "\x1b[38;5;201mhttp_read\x1b[0m ", 30);
    buffer_puts(buffer_2, "s=");

    buffer_putlong(buffer_2, h->sock);
    buffer_puts(buffer_2, " ret=");
    buffer_putlong(buffer_2, ret);
    if(ret < 0) {
      buffer_puts(buffer_2, " err=");
      buffer_putstr(buffer_2, http_strerror(h, ret));
    }
    if(ret < 0) {
      buffer_puts(buffer_2, " errno=");
      buffer_putstr(buffer_2, strerror(errno));
    }
    if(h->response->code != -1) {
      buffer_puts(buffer_2, " code=");
      buffer_putlong(buffer_2, r->code);
    }
    buffer_puts(buffer_2, " transfer=");
    buffer_puts(buffer_2, "HTTP_TRANSFER_");
    buffer_puts(buffer_2, ((const char* const[]){"UNDEF", "CHUNKED", "LENGTH", "BOUNDARY", 0})[r->transfer]);

    buffer_puts(buffer_2, " status=");
    buffer_puts(
        buffer_2,
        ((const char* const[]){"-1", "HTTP_RECV_HEADER", "HTTP_RECV_DATA", "HTTP_STATUS_CLOSED", "HTTP_STATUS_ERROR", "HTTP_STATUS_BUSY", "HTTP_STATUS_FINISH", 0})[r->status + 1]);

    if(ret > 0 && r->status == HTTP_RECV_DATA) {
      buffer_puts(buffer_2, " buf=");
      int len = MIN(ret, 30);
      buffer_putfmt(buffer_2, buf, len, &fmt_escapecharnonprintable);
      if(len < ret)
        buffer_puts(buffer_2, " {...}");
    }

    buffer_putnlflush(buffer_2);

    http_response_dump(r);
  }
#endif

  return ret;
}
