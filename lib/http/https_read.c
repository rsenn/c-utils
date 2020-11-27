#include "../http.h"

#ifdef HAVE_OPENSSL
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <assert.h>

ssize_t
https_read(fd_t fd, void* buf, size_t len, void* b) {
  http* h = ((buffer*)b)->cookie;
  char* msg = 0;
  ssize_t ret = 0;
  assert(h->tls);
  assert(h->connected);
  ret = https_tls2errno(h, SSL_read(h->ssl, buf, len));
  if(ret == 0) {
    ssize_t sret;
    sret = https_tls2errno(h, SSL_shutdown(h->ssl));
    if(sret == 1)
      SSL_clear(h->ssl);
  }
#if DEBUG_HTTP
  buffer_putspad(buffer_2, "https_read ", 18);
  buffer_puts(buffer_2, "sock=");
  buffer_putlong(buffer_2, h->sock);
  buffer_puts(buffer_2, " ret=");
  buffer_putlong(buffer_2, ret);
  if(ret <= 0) {
    buffer_puts(buffer_2, " err=");
    buffer_puts(buffer_2, https_errflag(SSL_get_error(h->ssl, ret)));
  }
  buffer_putnlflush(buffer_2);
#endif
  return ret;
}
#endif
