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
  int out = 0;
  errno = 0;
  if(!h->tls)
    return 0;
  if(!h->connected) {
    if((ret = http_ssl_connect(fd, h)) == 1) {
      io_wantread(fd);
      io_wantwrite(fd);
      errno = EAGAIN;
      ret = -1;
    }

    if(ret == -1 && (errno == EAGAIN || errno == EWOULDBLOCK))
      return ret;

    return ret;
  }
  assert(h->connected);
do_read:
  if((ret = SSL_read(h->ssl, buf, len)) <= 0) {
    buffer_puts(buffer_2, "SSL_read ");

    ret = http_ssl_error(ret, h, 0);

    out = 1;
  }

  return ret;
}
#endif