#include "../http.h"

#ifdef HAVE_OPENSSL
#include <openssl/ssl.h>
#include <openssl/err.h>

static void* http_sslctx;

int
https_socket(http* h) {
  if(!h->tls)
    return 0;
  if(http_sslctx == 0)
    http_sslctx = https_ssl_ctx();
  if((h->ssl = SSL_new(http_sslctx))) {
    SSL_set_fd(h->ssl, h->sock);
    return 0;
  }
  return -1;
}
#endif
