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

ssize_t
http_ssl_connect(fd_t fd, http* h) {
  ssize_t ret = SSL_connect(h->ssl);
  int err; /* it was not done */
  if(ret < 0) {
    char buf[256];
    size_t n = sizeof(buf);
    /* get error code */
    err = SSL_get_error(h->ssl, ret);

    ERR_error_string_n(err, buf, n);

    /* call ssl_read() again when socket gets readable */
    if(err == SSL_ERROR_WANT_READ) {
      errno = EAGAIN;
      return -1;
    }
    /* call ssl_read() again when socket gets writeable */
    else if(err == SSL_ERROR_WANT_WRITE) {
      errno = EWOULDBLOCK;
      return -1;
    }
    /*
     * EWOULDBLOCK, EINTR, EAGAIN are ignored because
     * these say the handshake is in progress and needs
     * more events.
     */
    else if(err == SSL_ERROR_SYSCALL) {
      /* ignore these */
      if(errno == EWOULDBLOCK || errno == EINTR || errno == EAGAIN) {
        //  errno = EAGAIN;
        return -1;
      }
      return -1;
    } else if(err == SSL_ERROR_ZERO_RETURN) {
      return 0;
    } else if(err == SSL_ERROR_SSL) {
      return 1;
    }

    if(err) {
      buffer_puts(buffer_2, "SSL error: ");
      buffer_puts(buffer_2, buf);
      buffer_putnlflush(buffer_2);
    }
    return -1;
  }
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
