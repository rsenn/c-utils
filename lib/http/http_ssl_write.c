#include "../http.h"

#ifdef HAVE_OPENSSL
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <assert.h>

ssize_t
http_ssl_write(fd_t fd, const void* buf, size_t n, void* b) {
  http* h = ((buffer*)b)->cookie;
  ssize_t ret;
  assert(h->tls);
  assert(h->connected);
  // do_write:
  ret = http_ssl2errno(h->ssl, SSL_write(h->ssl, buf, n));

#if DEBUG_OUTPUT
  buffer_puts(buffer_2, "http_ssl_write ");
  buffer_puts(buffer_2, " ret=");
  buffer_putulong(buffer_2, ret);
  if(ret <= 0) {
    buffer_puts(buffer_2, " err=");
    buffer_puts(buffer_2, http_ssl_errflag(SSL_get_error(h->ssl, ret)));
  }
  buffer_putnlflush(buffer_2);
#endif
  return ret;
}
#endif
