#include "../ssl_internal.h"
#include "../buffer.h"
#include <assert.h>

#ifdef HAVE_OPENSSL
#include <openssl/ssl.h>
#include <openssl/err.h>

void
ssl_instance_debug(ssl_instance* i) {
  int server, init;
  assert(i);
  assert(i->ssl);
  server = SSL_get_ssl_method(i->ssl) == ssl_server_method;
  init = SSL_in_init(i->ssl);

  buffer_putm_internal(buffer_2, "ssl ", server ? "server" : "client", "#", 0);
  buffer_putlong(buffer_2, SSL_get_fd(i->ssl));
  if(init)
    buffer_putm_internal(buffer_2, " (", server ? "accept" : "connect", ")", 0);
}
#endif
