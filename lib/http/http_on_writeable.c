#include "../http.h"
#include "../scan.h"
#include "../str.h"
#include "../stralloc.h"
#include "../io.h"
#include "../byte.h"
#include "../errmsg.h"
#include "../socket.h"
#include <errno.h>
#include <assert.h>

#ifdef HAVE_OPENSSL
#include <openssl/ssl.h>
#include <openssl/err.h>
#endif

ssize_t
http_on_writeable(http* h, void (*wantread)(fd_t)) {
  ssize_t ret = 0;
#if DEBUG_HTTP
  buffer_puts(buffer_2, "http_writable ");
  buffer_putlong(buffer_2, h->sock);
  buffer_puts(buffer_2, " tls=");
  buffer_putlong(buffer_2, !!h->tls);
  /*  buffer_puts(buffer_2, " ssl=");
    buffer_putptr(buffer_2, h->ssl);*/
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
#ifdef HAVE_OPENSSL
  if(h->ssl) {
    if(!h->connected) {
      ret = http_ssl2want(h, http_ssl_connect(h), wantread, 0);

      if(ret != 1)
        goto fail;
    }
  }
#endif
  // request:
  if(h->connected && h->sent == 0) {
    ret = http_ssl2want(h, http_sendreq(h), wantread, 0);

    if(h->sent)
      wantread(h->sock);
  }

fail:
  if(ret == -1 && h->response->err == EAGAIN)
    ret = 0;
#if DEBUG_HTTP
  buffer_puts(buffer_2, "http_on_writeable ");
  buffer_puts(buffer_2, " ret=");
  buffer_putlong(buffer_2, ret);
  buffer_puts(buffer_2, " err=");
  buffer_putlong(buffer_2, h->response->err);
  buffer_putnlflush(buffer_2);
#endif

  return ret;
}