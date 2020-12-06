#include "../http.h"
#include "../scan.h"
#include "../str.h"
#include "../stralloc.h"
#include "../io.h"
#include "../byte.h"
#include "../errmsg.h"
#include "../socket.h"
#include "../case.h"
#include "../tls.h"
#include <errno.h>
#include <assert.h>

static const char* const status_strings[] = (const char* const[]){"HTTP_RECV_HEADER",
                                                                  "HTTP_RECV_DATA",
                                                                  "HTTP_STATUS_CLOSED",
                                                                  "HTTP_STATUS_ERROR",
                                                                  "HTTP_STATUS_BUSY",
                                                                  "HTTP_STATUS_FINISH",
                                                                  0};
/**
 * @brief      Handle socket getting readable
 * @param      h     HTTP client
 * @return     -1 on error
 */
ssize_t
http_canread(http* h, void (*wantwrite)(fd_t)) {
  http_response* r;
  int err;
  ssize_t ret = 0;
#ifdef DEBUG_HTTP
  buffer_putspad(buffer_2, "http_canread ", 18);
  buffer_puts(buffer_2, "sock=");
  buffer_putlong(buffer_2, h->sock);
  buffer_puts(buffer_2, " tls=");
  buffer_putlong(buffer_2, !!h->tls);
  buffer_puts(buffer_2, " connected=");
  buffer_putlong(buffer_2, !!h->connected);
  buffer_puts(buffer_2, " keepalive=");
  buffer_putlong(buffer_2, !!h->keepalive);
  buffer_puts(buffer_2, " nonblocking=");
  buffer_putlong(buffer_2, !!h->nonblocking);
  buffer_puts(buffer_2, " sent=");
  buffer_putlong(buffer_2, !!h->sent);
  buffer_putsflush(buffer_2, "\n");
#endif
  if(h->tls) {
    if(!h->connected) {
      if((ret = tls_connect(h->sock)) != 1)
        goto fail;
      h->connected = 1;
    }
    if(h->connected && h->sent == 0) {
      http_sendreq(h);
      tls_want(h->sock, 0, wantwrite);
    }
  }

  /*if(h->q.in.p == h->q.in.n)*/ {
    if((ret = buffer_freshen(&h->q.in)) == 0)
      return 0;
  }
#ifdef DEBUG_HTTP
  buffer_puts(buffer_2, "buffer_freshen ret=");
  buffer_putlong(buffer_2, ret);
  buffer_putnlflush(buffer_2);
#endif

  if((r = h->response) == NULL)
    return ret;

  while(r->status == HTTP_RECV_HEADER) {
    size_t pos = r->data.len;

    if((ret = buffer_getline_sa(&h->q.in, &r->data)) <= 0)
      break;
    stralloc_trimr(&r->data, "\r\n", 2);
    stralloc_nul(&r->data);
    if(r->data.len == 0) {
      r->ptr = 0;
      r->status = HTTP_RECV_DATA;
      if(h->q.in.p < h->q.in.n) {
        ret = 1;
      } else {
        ret = 0;
        errno = EWOULDBLOCK;
      }
      break;
    }
    if(case_starts(&r->data.s[pos], "Content-Type: multipart")) {
      size_t p = pos + str_find(&r->data.s[pos], "boundary=");
      if(r->data.s[p]) {
        stralloc_copys(&r->boundary, &r->data.s[p + str_len("boundary=")]);
      }
      r->transfer = HTTP_TRANSFER_BOUNDARY;
    } else if(case_starts(&r->data.s[pos], "Content-Length: ")) {
      scan_ulonglong(&r->data.s[pos + 16], &r->content_length);
      r->transfer = HTTP_TRANSFER_LENGTH;
    } else {
      r->transfer = HTTP_TRANSFER_CHUNKED;
    }
    stralloc_zero(&r->data);
  }
  if(ret == -1) {
    err = errno;
    errno = 0;
  } else {
    err = 0;
  }

fail:
  if(ret == -1 && !(errno == EAGAIN || errno == EWOULDBLOCK))
    r->status = HTTP_STATUS_ERROR;
  if(ret == -1 && (errno == EAGAIN || errno == EWOULDBLOCK))
    tls_want(h->sock, 0, wantwrite);

#ifdef DEBUG_HTTP
  buffer_putspad(buffer_2, "http_canread ", 18);
  buffer_puts(buffer_2, "sock=");
  buffer_putlong(buffer_2, h->sock);
  buffer_puts(buffer_2, " status=");
  buffer_puts(buffer_2,
              ((const char* const[]){"-1",
                                     "HTTP_RECV_HEADER",
                                     "HTTP_RECV_DATA",
                                     "HTTP_STATUS_CLOSED",
                                     "HTTP_STATUS_ERROR",
                                     "HTTP_STATUS_BUSY",
                                     "HTTP_STATUS_FINISH",
                                     0})[h->response->status + 1]);
  buffer_puts(buffer_2, " ret=");
  buffer_putlong(buffer_2, ret);
  buffer_puts(buffer_2, " err=");
  buffer_puts(buffer_2, http_strerror(h, ret));
  buffer_putnlflush(buffer_2);
#endif
  return ret;
}
