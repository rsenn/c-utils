#include "../ssl_internal.h"
#include <assert.h>

#ifdef HAVE_OPENSSL
#include <openssl/ssl.h>
#include <openssl/err.h>

int
ssl_accept(fd_t fd) {
  ssl_instance* i = iarray_get(&ssl_list, fd);
  assert(i);
  assert(i->ssl);

  return ssl_instance_return(i, SSL_accept(i->ssl));
}
#endif
