#include "../http.h"

#ifdef HAVE_OPENSSL
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <assert.h>

ssize_t
https_write(fd_t fd, const void* buf, size_t n, void* b) {
  http* h = ((buffer*)b)->cookie;
  ssize_t ret;
  assert(h->tls);
  assert(h->connected);
  ret = https_tls2errno(h, SSL_write(h->ssl, buf, n));
#if DEBUG_HTTP
  buffer_putspad(buffer_2, "https_write ", 18);
  buffer_puts(buffer_2, "sock=");
  buffer_putulong(buffer_2, h->sock);
  buffer_puts(buffer_2, " ret=");
  buffer_putulong(buffer_2, ret);
  if(ret <= 0) {
    buffer_puts(buffer_2, " err=");
    buffer_puts(buffer_2, https_errflag(SSL_get_error(h->ssl, ret)));
  }
  buffer_putnlflush(buffer_2);
#endif
  return ret;
}
#endif
