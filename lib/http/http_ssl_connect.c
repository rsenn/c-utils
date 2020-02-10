#include "../http.h"

#ifdef HAVE_OPENSSL
#include <openssl/ssl.h>
#include <assert.h>
#include <openssl/err.h>

ssize_t
http_ssl_connect(fd_t fd, http* h) {
  ssize_t ret;
  char* msg = 0;

  assert(!h->connected);
  ret = SSL_connect(h->ssl);

  if(ret <= 0)
    ret = http_ssl_error(ret, h, &msg);

  if(!(ret == -1 && (errno == EWOULDBLOCK || errno == EAGAIN))) {
  }

  buffer_puts(buffer_2, "SSL connect: ret=");
  buffer_putlong(buffer_2, ret);
  buffer_putnlflush(buffer_2);

  if(ret == 1)
    h->connected = 1;

  return ret;
}
#endif