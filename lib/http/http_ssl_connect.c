#include "../http.h"

#ifdef HAVE_OPENSSL
#include <openssl/ssl.h>
#include <openssl/err.h>

ssize_t
http_ssl_connect(fd_t fd, http* h) {
  ssize_t ret = SSL_connect(h->ssl);

  buffer_puts(buffer_2, "SSL connect = ");
  buffer_putlong(buffer_2, ret);
  buffer_putnlflush(buffer_2);

  if(ret == 1)
    h->connected = 1;
  else if(ret <= 0)
    ret = http_ssl_error(ret, h, 0);

  return ret;
}
#endif