#include "../http.h"

#ifdef HAVE_OPENSSL
#include <openssl/ssl.h>
#include <openssl/err.h>

int
http_ssl_socket(http* h) {
  if(http_sslctx == 0)
    http_sslctx = http_ssl_ctx();

  if((h->ssl = SSL_new(http_sslctx))) {
    SSL_set_fd(h->ssl, h->sock);
    return 0;
  }
  return -1;
}

#endif
