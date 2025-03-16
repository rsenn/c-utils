#define NO_BUILTINS
#include "../http.h"
#include "../scan.h"
#include "../socket_internal.h"
#include "../str.h"
#include "../stralloc.h"
#include "../io.h"
#include "../byte.h"
#include "../buffer.h"
#include "../fmt.h"
#include "../unix.h"
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
static int seq = 0;

ssize_t
http_read(fd_type fd, char* x, size_t n, void* headers_len) {
  http* h = ((buffer*)headers_len)->cookie ? ((buffer*)headers_len)->cookie : (http*)(ptrdiff_t)fd;
  buffer* in = &h->q.in;
  http_response* response = h->response;
  http_status st = response->status;
  ssize_t r, ret = 0 /*, received, oldlen*/;

  ++seq;

  if(!n)
    return 0;

again:
  // oldlen = buffer_LEN(in);
  r = response->status == HTTP_RECV_HEADER ? buffer_freshen(in) : buffer_feed(in);
  int err = errno;

  if(r < 0 && err != EAGAIN && err != EWOULDBLOCK)
    response->status = HTTP_STATUS_ERROR;
  else if(/*r == received ||*/ r == 0)
    response->status = HTTP_STATUS_CLOSED;

  if(r <= 0) {
    if(r < 0)
      if((int)response->status == st) {
        if(response->err != 0) {
          errno = response->err;
          ret = -1;
        }
      }
    goto end;
  }

  //  received = buffer_LEN(in) - oldlen;

  if((response->status == HTTP_RECV_HEADER || response->status == HTTP_RECV_DATA)) {

    ret = http_read_internal(h->sock, x, n, &h->q.in);

    /* if(response->status == HTTP_RECV_DATA && ret == 0)
       goto again;*/
    if(response->status == HTTP_RECV_DATA && ret == 0) {
      ret = -1;
      errno = EAGAIN;
    }
    goto end;
  } else if(response->status == HTTP_STATUS_FINISH) {
    goto end;
  } else if(response->status == HTTP_STATUS_CLOSED) {
    goto end;
  } else if(response->status == HTTP_STATUS_ERROR) {
    goto end;
  }

  /* if(response->status >= HTTP_STATUS_CLOSED)
     http_close(h);*/

  /*if(response->status == HTTP_STATUS_FINISH || response->status == HTTP_STATUS_CLOSED) {
  }*/

  if(response->code == 302) {
    size_t pos = 0, hlen, end;
    const char* location = http_get_header(response->data.s, response->headers_len, "Location", &hlen);
    end = hlen;

    if((pos = byte_finds(location, hlen, "://"))) {
      pos += 3;
      hlen -= pos;
    }

    pos += byte_chr(&location[pos], hlen, '/');

    if(http_get(h, &location[pos])) {
      io_onlywantwrite(h->sock);
      errno = EAGAIN;
      ret = -1;
    }
  }

end:

#ifdef DEBUG_HTTP
  if(response->status == HTTP_STATUS_BUSY || response->status == HTTP_RECV_HEADER || response->status == HTTP_RECV_DATA) {
    buffer_putspad(buffer_2, "\x1b[38;5;201mhttp_read\x1b[0m ", 30);

    buffer_puts(buffer_2, "seq=");
    buffer_putlong(buffer_2, seq);

    buffer_puts(buffer_2, " s=");
    buffer_putlong(buffer_2, h->sock);

    buffer_puts(buffer_2, " ret=");
    buffer_putlong(buffer_2, ret);

    if(ret < 0) {
      buffer_puts(buffer_2, " err=");
      buffer_putstr(buffer_2, http_strerror(h, ret));
    }

    if(ret < 0) {
      buffer_puts(buffer_2, " errno=");
      buffer_puts(buffer_2, unix_errno(errno));
    }

    if(h->response->code != -1) {
      buffer_puts(buffer_2, " code=");
      buffer_putlong(buffer_2, response->code);
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
                })[response->transfer]);
    buffer_puts(buffer_2, " status=");
    buffer_puts(buffer_2,
                ((const char* const[]){
                    "0", "HTTP_RECV_HEADER", "HTTP_RECV_DATA", "HTTP_STATUS_CLOSED", "HTTP_STATUS_ERROR", "HTTP_STATUS_BUSY", "HTTP_STATUS_FINISH", 0})[response->status]);

    if(ret > 0 && response->status == HTTP_RECV_DATA) {
      int n = MIN(ret, 30);

      buffer_puts(buffer_2, " x=");
      buffer_putfmt(buffer_2, x, n, &fmt_escapecharnonprintable);

      if(n < ret)
        buffer_puts(buffer_2, " {...}");
    }

    buffer_putnlflush(buffer_2);

    // http_response_dump(response);
  }
#endif

  return ret;
}
