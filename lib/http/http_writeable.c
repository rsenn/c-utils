#include "../http.h"
#include "../scan.h"
#include "../socket_internal.h"
#include "../str.h"
#include "../stralloc.h"
#include "../io.h"
#include "../byte.h"
#include <errno.h>
#include <assert.h>

#ifdef HAVE_OPENSSL
#include <openssl/ssl.h>
#include <openssl/err.h>
ssize_t http_ssl_error(ssize_t ret, http* h);
ssize_t
http_ssl_connect(fd_t fd, http* h) {
  ssize_t ret = SSL_connect(h->ssl);
  if(ret < 0)
    ret = http_ssl_error(ret, h);
  return ret;
}
#endif

ssize_t
http_writeable(http* h) {
  ssize_t ret = 0;
#ifdef HAVE_OPENSSL
  if(h->ssl) {
    if(!h->connected) {
      if((ret = http_ssl_connect(h->sock, h)) == 1) {
        h->connected = 1;

        buffer_puts(buffer_2, "SSL connected");
        buffer_putnlflush(buffer_2);

        if(!io_canwrite(h->sock)) {

          errno = EWOULDBLOCK;
          return -1;
        }

      } else if(ret == -1) {
        if(errno == EAGAIN) {
          io_wantread(h->sock);
        } else if(errno == EWOULDBLOCK) {
          io_wantwrite(h->sock);
        }
        errno = EWOULDBLOCK;
        return ret;
      }
    }
  }
#endif
  h->connected = 1;
  http_sendreq(h);
  io_dontwantwrite(h->sock);
}
