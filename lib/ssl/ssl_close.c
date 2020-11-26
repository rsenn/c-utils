#include "../ssl_internal.h"
#include <assert.h>

#ifdef HAVE_OPENSSL
#include <openssl/ssl.h>
#include <openssl/err.h>

int
ssl_close(fd_t fd) {
  ssl_instance* inst = iarray_get(&ssl_list, fd);
  assert(inst);
  assert(inst->ssl);

  return ssl_instance_return(inst, SSL_shutdown(inst->ssl));
}
#endif
