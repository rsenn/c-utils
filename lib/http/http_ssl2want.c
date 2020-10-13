#include "../http.h"

#ifdef HAVE_OPENSSL
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <assert.h>
#include <errno.h>

ssize_t
http_ssl2want(http* h, ssize_t ret, void (*wantread)(fd_t), void (*wantwrite)(fd_t)) {

  if(ret <= 0) {
    switch(SSL_get_error(h->ssl, ret)) {
      case SSL_ERROR_WANT_READ:
        assert(wantread);
        wantread(h->sock);
        break;
      case SSL_ERROR_WANT_WRITE:
        assert(wantwrite);
        wantwrite(h->sock);
        break;
      default: break;
    }
  }
  return ret;
}
#endif