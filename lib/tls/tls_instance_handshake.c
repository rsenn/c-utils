#include "../tls_internal.h"
#include "../buffer.h"
#include <assert.h>

#ifdef HAVE_OPENSSL
#include <openssl/ssl.h>
#include <openssl/err.h>
#ifndef TLS_ST_OK
#ifdef SSL_ST_OK
#define TLS_ST_OK SSL_ST_OK
#endif
#endif

int
tls_instance_handshake(tls_instance_t* i) {
  int ret;
  assert(i->ssl);

  if(SSL_get_state(i->ssl) != TLS_ST_OK) {
    int server = SSL_get_ssl_method(i->ssl) == tls_server_method;

    int (*sslfn)(tls_t*);
#ifdef DEBUG_OUTPUT_
    tls_instance_debug(i);
    buffer_putsflush(buffer_2, " continuing handshake...\n");
#endif
    sslfn = server ? SSL_accept : SSL_connect;

    ret = tls_instance_return(i, server ? TLS_OP_ACCEPT : TLS_OP_CONNECT, sslfn(i->ssl));
#ifdef DEBUG_OUTPUT
    tls_instance_debug(i);
    buffer_puts(buffer_2, " resumed handshake: ");
    buffer_putlong(buffer_2, ret);
    if(ret == -1)
      buffer_putm_internal(buffer_2, " (", tls_instance_strerror(i), ")", NULL);

    buffer_putnlflush(buffer_2);
#endif
  } else {
    ret = 1;
  }

  return ret;
}
#endif
