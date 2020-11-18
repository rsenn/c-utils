#include "../http.h"
#include "../io.h"

#ifdef HAVE_OPENSSL
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <assert.h>

ssize_t
http_ssl_connect(http* h) {
  ssize_t ret;
  char* msg = 0;
  assert(!h->connected);
  ret = http_ssl2errno(h, SSL_connect(h->ssl));

  if(ret == 1) {
    h->connected = 1;
#if DEBUG_HTTP
    buffer_putsflush(buffer_2, "http_ssl_connect done!\n");
#endif
  }
#if DEBUG_HTTP
  buffer_putspad(buffer_2, "http_ssl_connect ", 18);
  buffer_puts(buffer_2, "sock=");
  buffer_putlong(buffer_2, h->sock);
  buffer_puts(buffer_2, " ret=");
  buffer_putlong(buffer_2, ret);

  if(errno) {
    buffer_puts(buffer_2, " errno=");
    buffer_putlong(buffer_2, errno);
  }
  buffer_puts(buffer_2, " err=");
  buffer_puts(buffer_2, http_ssl_errflag(SSL_get_error(h->ssl, ret)));
  buffer_putnlflush(buffer_2);

#endif
  return ret;
}
#endif