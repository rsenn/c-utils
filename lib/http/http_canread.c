#define NO_BUILTINS
#include "../http.h"
#include "../scan.h"
#include "../str.h"
#include "../stralloc.h"
#include "../buffer.h"
#include "../fmt.h"
#include "../io.h"
#include "../byte.h"
#include "../errmsg.h"
#include "../socket.h"
#include "../case.h"
#include "../tls.h"
#include "../unix.h"
#include <errno.h>
#include <assert.h>
#include <string.h>

/**
 * @brief      Handle socket getting readable
 * @param      h     HTTP client
 * @return     -1 on error
 */
ssize_t
http_canread(http* h, void (*wantread)(fd_type), void (*wantwrite)(fd_type)) {
  http_response* r;
  int err;
  size_t n;
  ssize_t ret = 0, received;

  if(h->tls) {
    if(!h->connected) {

      ret = tls_connect(h->sock);

      if(ret == -1) {
        h->err = tls_errno(h->sock);
        tls_want(h->sock, wantread, wantwrite);
      }

      if(ret != 1)
        goto fail;

      h->connected = 1;
    }

    if(h->connected && h->sent == 0) {
      ret = http_sendreq(h);

      if(ret == -1)
        tls_want(h->sock, wantread, wantwrite);

      if(ret <= 0)
        goto fail;

      h->sent = 1;
    }

  } else {
    if(!h->connected)
      h->connected = 1;
  }

  n = buffer_LEN(&h->q.in);

  if((ret = buffer_freshen(&h->q.in)) <= 0) {
    // if(!(ret == -1 && errno == EAGAIN))
    goto fail;
  }

  received = ret > 0 ? ret - n : 0;

#ifdef DEBUG_HTTP
  buffer_putspad(buffer_2, "\x1b[1;32mhttp_canread\x1b[0m(1)", 30);
  buffer_puts(buffer_2, "buffer_freshen() = ");
  buffer_putlong(buffer_2, ret);
  buffer_putnlflush(buffer_2);
#endif

  if((r = h->response) == NULL)
    goto fail;

  while(r->status == HTTP_RECV_HEADER) {
    size_t len, pos = r->data.len;

    if((ret = buffer_getline_sa(&h->q.in, &r->data)) <= 0)
      break;

    // stralloc_trimr(&r->data, "\r\n", 2);

    len = byte_trimr(&r->data.s[pos], r->data.len - pos, "\r\n", 2);

    stralloc_nul(&r->data);

    if(len == 0) {
      r->headers_len = r->data.len;
      r->status = HTTP_RECV_DATA;

      if(h->q.in.p < h->q.in.n) {
        ret = 1;
      } else {
        ret = 0;
        errno = EWOULDBLOCK;
      }

      break;
    }

    if(!case_diffb(&r->data.s[pos], str_len("Content-Type: multipart"), "Content-Type: multipart")) {
      size_t p = pos + str_find(&r->data.s[pos], "boundary=");

      if(r->data.s[p])
        stralloc_copys(&r->boundary, &r->data.s[p + str_len("boundary=")]);

      r->transfer = HTTP_TRANSFER_BOUNDARY;
    } else if(!case_diffb(&r->data.s[pos], str_len("Content-Length: "), "Content-Length: ")) {
      scan_ulonglong(&r->data.s[pos + 16], &r->content_length);
      r->transfer = HTTP_TRANSFER_LENGTH;
    } else {
      r->transfer = HTTP_TRANSFER_CHUNKED;
      r->content_length = 0;
      r->chunk_length = 0;
    }

    if(r->header) {
      r->header(h, r->data.s + pos, len);
    }

    /*   r->headers_len = 0;
       stralloc_zero(&r->data);*/
  }

  if(r->status == HTTP_RECV_HEADER || r->status == HTTP_RECV_DATA) {
    if(ret > 0) {
      stralloc_readyplus(&r->data, ret);
      buffer_get(&h->q.in, &r->data.s[r->data.len], ret);
      r->data.len += ret;

#ifdef DEBUG_HTTP
      buffer_putspad(buffer_2, "\x1b[1;32mhttp_canread\x1b[0m(2) DATA ", 30);
      buffer_puts(buffer_2, "s=");
      buffer_putlong(buffer_2, h->sock);
      buffer_puts(buffer_2, " ret=");
      buffer_putlong(buffer_2, ret);

      buffer_puts(buffer_2, " r->data.len=");
      buffer_putulonglong(buffer_2, r->data.len);
      buffer_putnlflush(buffer_2);
#endif
    }
  }

fail:
  if(ret == -1)
    err = h->err = h->tls ? tls_errno(h->sock) : errno;
  else
    err = h->err = 0;

  if(h->tls) {
    if(err == EAGAIN || err == EWOULDBLOCK) {
      tls_want(h->sock, wantread, wantwrite);
      errno = err;
    }
  }

#ifdef DEBUG_HTTP
  buffer_putspad(buffer_2, "\x1b[1;32mhttp_canread\x1b[0m(3)", 30);
  buffer_puts(buffer_2, " ret=");
  buffer_putlong(buffer_2, ret);
  buffer_puts(buffer_2, " err=");
  buffer_putstr(buffer_2, http_strerror(h, ret));
  http_dump(h);
#endif

  return ret;
}
