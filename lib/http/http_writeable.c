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
#endif

ssize_t
http_ssl_connect(fd_t fd, http* h) {
  ssize_t ret = SSL_connect(h->ssl);
  int err; /* it was not done */
  if(ret < 0) {
    /* get error code */
    err = SSL_get_error(h->ssl, ret);
    /* call ssl_read() again when socket gets readable */
    if(err == SSL_ERROR_WANT_READ) {
      io_wantread(fd);
      errno = EAGAIN;
      return -1;
    }
    /* call ssl_read() again when socket gets writeable */
    if(err == SSL_ERROR_WANT_WRITE) {
      io_wantwrite(fd);
      errno = EAGAIN;
      return -1;
    }
    /*
     * EWOULDBLOCK, EINTR, EAGAIN are ignored because
     * these say the handshake is in progress and needs
     * more events.
     */
    if(err == SSL_ERROR_SYSCALL) {
      /* ignore these */
      if(errno == EWOULDBLOCK || errno == EINTR || errno == EAGAIN) {
        errno = EAGAIN;
        return -1;
      }
      return -1;
    }
    if(err == SSL_ERROR_ZERO_RETURN)
      return 0;
  }
  return ret;
}
void
http_writeable(http* h) {
#ifdef HAVE_OPENSSL
  if(h->ssl) {
    if(!h->connected) {
      if(http_ssl_connect(h->sock, h) != 1)
        return;
    }
  }
#endif
  h->connected = 1;
  http_sendreq(h);
}
