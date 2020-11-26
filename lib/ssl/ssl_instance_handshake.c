#include "../ssl_internal.h"
#include "../buffer.h"
#include <assert.h>

#ifdef HAVE_OPENSSL
#include <openssl/ssl.h>
#include <openssl/err.h>

int
ssl_instance_handshake(ssl_instance* i) {
  int ret;
  assert(i->ssl);

  if(SSL_get_state(i->ssl) != TLS_ST_OK) {
    int server = SSL_get_ssl_method(i->ssl) == ssl_server_method;

    int (*sslfn)(ssl_t*);
#ifdef DEBUG_OUTPUT_
    ssl_instance_debug(i);
    buffer_putsflush(buffer_2, " continuing handshake...\n");
#endif
    sslfn = server ? SSL_accept : SSL_connect;

    ret = ssl_instance_return(i, sslfn(i->ssl));
#ifdef DEBUG_OUTPUT
    ssl_instance_debug(i);
    buffer_puts(buffer_2, " resumed handshake: ");
    buffer_putlong(buffer_2, ret);
    if(ret == -1)
      buffer_putm_internal(buffer_2, " (", ssl_instance_error(i), ")", 0);

    buffer_putnlflush(buffer_2);
#endif
  } else {
    ret = 1;
  }

  return ret;
}
#endif
