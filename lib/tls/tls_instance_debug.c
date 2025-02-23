#include "../tls_internal.h"
#include "../buffer.h"
#include <assert.h>

#ifdef HAVE_OPENSSL
#include <openssl/ssl.h>
#include <openssl/err.h>

void
tls_instance_debug(tls_instance_t* i) {
  int server, init;
  assert(i);
  assert(i->ssl);
  server = SSL_get_ssl_method(i->ssl) == tls_server_method;
  init = SSL_in_init(i->ssl);

  buffer_putm_internal(buffer_2, "ssl ", server ? "server" : "client", "#", NULL);
  buffer_putlong(buffer_2, SSL_get_fd(i->ssl));
  if(init)
    buffer_putm_internal(buffer_2, " (", server ? "accept" : "connect", ")", NULL);
}
#endif
