#include "../http.h"
#include "../io.h"
#include "../socket.h"
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
  if(h->tls) {
    tls_close(h->sock);
    h->ssl = NULL;
  }

  if(h->sock != -1) {
    int ret;
    ret = socket_close(h->sock);
    io_close(h->sock);
#if DEBUG_HTTP
    buffer_putspad(buffer_2, "http_close ", 18);
    buffer_puts(buffer_2, " sock=");
    buffer_putlong(buffer_2, h->sock);
    buffer_puts(buffer_2, " ret=");
    buffer_putlong(buffer_2, ret);
    buffer_putnlflush(buffer_2);
#endif
    h->sock = -1;
  }
}
