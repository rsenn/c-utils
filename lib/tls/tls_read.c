#include "../tls_internal.h"
#include "../buffer.h"
#include <assert.h>

#ifdef HAVE_OPENSSL
#include <openssl/ssl.h>
#include <openssl/err.h>

ssize_t
tls_read(fd_t fd, void* data, size_t len) {
  ssize_t ret;
  tls_instance* i = iarray_get(&tls_list, fd);
  assert(i);
  assert(i->ssl);

  if(!SSL_is_init_finished(i->ssl)) {
    if((ret = tls_instance_handshake(i)) != 1) {
      errno = tls_instance_errno(i);
      return ret;
    }
  }

  if((ret = tls_instance_return(i, SSL_read(i->ssl, data, len))) <= 0)
    errno = tls_instance_errno(i);

  return ret;
}
#endif
