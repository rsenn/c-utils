#include "../ssl_internal.h"
#include <assert.h>

#ifdef HAVE_OPENSSL
#include <openssl/ssl.h>
#include <openssl/err.h>

ssize_t
ssl_read(fd_t fd, void* data, size_t len) {
  ssize_t ret;
  ssl_instance* inst = iarray_get(&ssl_list, fd);
  assert(inst);
  assert(inst->ssl);

  ret = ssl_instance_return(inst, SSL_read(inst->ssl, data, len));

  return ret;
}
#endif
