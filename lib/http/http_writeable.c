#include "../http.h"
#include "../scan.h"
#include "../str.h"
#include "../stralloc.h"
#include "../io.h"
#include "../byte.h"
#include "../errmsg.h"
#include <errno.h>
#include <assert.h>

#ifdef HAVE_OPENSSL
#include <openssl/ssl.h>
#include <openssl/err.h>
#endif

ssize_t
http_writeable(http* h) {
  ssize_t ret = 0;

#ifdef DEBUG_OUTPUT_HTTP
  buffer_puts(buffer_2, "http writable ");
  buffer_puts(buffer_2, "tls=");
  buffer_putlong(buffer_2, !!h->tls);
  buffer_puts(buffer_2, " sock=");
  buffer_putlong(buffer_2, h->sock);
  buffer_puts(buffer_2, " ssl=");
  buffer_putptr(buffer_2, h->ssl);
  buffer_puts(buffer_2, " connected=");
  buffer_putlong(buffer_2, !!h->connected);
  buffer_puts(buffer_2, " keepalive=");
  buffer_putlong(buffer_2, !!h->keepalive);
  buffer_puts(buffer_2, " nonblocking=");
  buffer_putlong(buffer_2, !!h->nonblocking);
  buffer_putsflush(buffer_2, "\n");
#endif

#ifdef HAVE_OPENSSL
  if(h->ssl) {
    if(!h->connected) {
      if((ret = http_ssl_connect(h->sock, h)) <= 0) {
        if((ret = http_ssl_error(ret, h, 0)) == -1 && (errno == EWOULDBLOCK || errno == EAGAIN))
          return ret;
      }

      return ret;
    }
  }
#endif
request:
  h->connected = 1;
  if(h->connected) {
    http_sendreq(h);
    io_dontwantwrite(h->sock);
    io_wantread(h->sock);
  }
}
