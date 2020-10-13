#include "../http.h"

#ifdef HAVE_OPENSSL
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <assert.h>

ssize_t
http_ssl_read(fd_t fd, void* buf, size_t len, void* b) {
  http* h = ((buffer*)b)->cookie;
  char* msg = 0;
  ssize_t ret = 0;
  assert(h->tls);
  assert(h->connected);
  // do_read:
  ret = http_ssl2errno(h, SSL_read(h->ssl, buf, len));

  if(ret == 0) {
    ssize_t sret;
    sret = http_ssl2errno(h, SSL_shutdown(h->ssl));

    if(sret == 1)
      SSL_clear(h->ssl);
  }
#if DEBUG_HTTP
  buffer_puts(buffer_2, "http_ssl_read ");
  buffer_puts(buffer_2, " ret=");
  buffer_putlong(buffer_2, ret);
  if(ret <= 0) {
    buffer_puts(buffer_2, " err=");
    buffer_puts(buffer_2, http_ssl_errflag(SSL_get_error(h->ssl, ret)));
  }
  buffer_putnlflush(buffer_2);
#endif
  return ret;
}
#endif
