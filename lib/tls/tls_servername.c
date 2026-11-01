#include "../tls_internal.h"

#ifdef HAVE_OPENSSL
#include <openssl/ssl.h>

/* Sets the server name that a client handshake on fd announces (SNI).
 * Returns 1 on success, 0 if there is no TLS instance or it was refused. */
int
tls_servername(fd_type fd, const char* name) {
  tls_instance_t* i = iarray_get(&tls_list, fd);

  if(i == NULL || i->ssl == NULL || name == NULL)
    return 0;

  return SSL_set_tlsext_host_name(i->ssl, name) == 1;
}
#endif
