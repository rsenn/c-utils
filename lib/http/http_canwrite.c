#include "../http.h"
#include "../scan.h"
#include "../str.h"
#include "../stralloc.h"
#include "../buffer.h"
#include "../io.h"
#include "../byte.h"
#include "../errmsg.h"
#include "../socket.h"
#include "../fmt.h"
#include <errno.h>
#include <assert.h>
#include <string.h>

ssize_t
http_canwrite(http* h, void (*wantread)(fd_t)) {
  ssize_t ret = 0;
  if(h->tls) {
    if(!h->connected) {

      ret = tls_connect(h->sock);
      if(ret == -1)
        tls_want(h->sock, wantread, 0);

      if(ret != 1)
        goto fail;
      h->connected = 1;
    }
  } else {
    if(!h->connected)
      h->connected = 1;
  }
  if(h->connected && h->sent == 0) {
    ret = http_sendreq(h);
    if(ret == -1)
      tls_want(h->sock, wantread, 0);
    if(ret <= 0)
      goto fail;
    h->sent = 1;
  }
fail:
  /* if(ret == -1 && !(errno == EAGAIN || errno == EWOULDBLOCK))
     if(h->response)
       h->response->status = HTTP_STATUS_ERROR;*/
  if(ret == -1 && (errno == EAGAIN || errno == EWOULDBLOCK))
    tls_want(h->sock, wantread, 0);

#ifdef DEBUG_HTTP
  buffer_putspad(buffer_2, "http_canwrite ", 30);
  buffer_puts(buffer_2, "s=");
  buffer_putlong(buffer_2, h->sock);
  if(h->tls) {
    buffer_puts(buffer_2, " tls=");
    buffer_putlong(buffer_2, !!h->tls);
  }
  if(h->connected) {
    buffer_puts(buffer_2, " connected=");
    buffer_putlong(buffer_2, !!h->connected);
  }
  if(h->keepalive) {
    buffer_puts(buffer_2, " keepalive=");
    buffer_putlong(buffer_2, !!h->keepalive);
  }
  if(h->nonblocking) {
    buffer_puts(buffer_2, " nonblocking=");
    buffer_putlong(buffer_2, !!h->nonblocking);
  }
  if(h->sent) {
    buffer_puts(buffer_2, " sent=");
    buffer_putlong(buffer_2, !!h->sent);
  }
  if(h->response->code != -1) {
    buffer_puts(buffer_2, " code=");
    buffer_putlong(buffer_2, h->response->code);
  }
  if(h->response->data.len > 0) {
    buffer_puts(buffer_2, " data=");
    buffer_put_escaped(buffer_2, h->response->data.s, h->response->data.len, &fmt_escapecharnonprintable);
  }
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
  /* buffer_puts(buffer_2, " tls=");
    buffer_putlong(buffer_2, !!h->tls);
    buffer_puts(buffer_2, " connected=");
    buffer_putlong(buffer_2, !!h->connected);*/
  if(h->response->code != -1) {
    buffer_puts(buffer_2, " code=");
    buffer_putlong(buffer_2, h->response->code);
  }
  buffer_puts(buffer_2, " status=");
  buffer_puts(buffer_2, ((const char* const[]){"-1", "HTTP_RECV_HEADER", "HTTP_RECV_DATA", "HTTP_STATUS_CLOSED", "HTTP_STATUS_ERROR", "HTTP_STATUS_BUSY", "HTTP_STATUS_FINISH", 0})[h->response->status + 1]);
  buffer_putnlflush(buffer_2);
#endif
  return ret;
}
