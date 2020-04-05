#include "../http.h"
#include "../windoze.h"
#if WINDOWS_NATIVE
#include <io.h>
#else
#include <unistd.h>
#include <netdb.h>
#endif

#ifdef HAVE_OPENSSL
#include <openssl/opensslconf.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#endif

void
http_close(http* h) {
#ifdef HAVE_OPENSSL
  if(h->ssl) {
    SSL_shutdown(h->ssl);
    h->ssl = NULL;
  } else
#endif
      if(h->sock != -1) {
    close(h->sock);
    h->sock = -1;
  }

  h->sock = -1;
}
