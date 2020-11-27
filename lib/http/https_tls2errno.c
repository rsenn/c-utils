#include "../http.h"

#ifdef HAVE_OPENSSL
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <assert.h>
#include <errno.h>

ssize_t
https_tls2errno(http* h, ssize_t ret) {
  int sslerr;
  errno = 0;
  if(ret <= 0) {
    sslerr = SSL_get_error(h->ssl, ret);
    switch(sslerr) {
      case SSL_ERROR_WANT_READ: errno = EAGAIN; break;
      case SSL_ERROR_WANT_WRITE: errno = EWOULDBLOCK; break;
      case SSL_ERROR_ZERO_RETURN: errno = EIO; break;
      case SSL_ERROR_SSL: errno = EIO; break;
      case SSL_ERROR_NONE: errno = 0; break;
      default: break;
    }
  }
  if(h->response)
    h->response->err = errno;
  return ret;
}
#endif
