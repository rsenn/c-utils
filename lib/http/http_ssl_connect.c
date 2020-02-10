#include "../http.h"

#ifdef HAVE_OPENSSL
#include <openssl/ssl.h>
#include <assert.h>
#include <openssl/err.h>

ssize_t
http_ssl_connect(fd_t fd, http* h) {
  ssize_t ret;
  char* msg = 0;
  int out = 0;
  errno = 0;
  assert(!h->connected);
  ret = SSL_connect(h->ssl);
  if(ret <= 0) {
    buffer_puts(buffer_2, "SSL_connect ");
    out = 1;
    ret = http_ssl_error(ret, h, 0);
    /*  if(ret == -1  && (errno == EWOULDBLOCK || errno == EAGAIN))
        return ret;*/
  }

  if(ret == 1) {
    h->connected = 1;
    io_wantwrite(fd);
  }
  return ret;
}
#endif