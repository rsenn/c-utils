#include "../tls_internal.h"
#include <assert.h>

#ifdef HAVE_OPENSSL
#include <openssl/ssl.h>
#include <openssl/err.h>

int
tls_want(fd_t fd, void (*wantread)(fd_t), void (*wantwrite)(fd_t)) {
  tls_instance* inst = tls_instance_get(fd);

  switch(inst->error) {
    case SSL_ERROR_WANT_READ:
      assert(wantread);
      wantread(fd);
      break;
    case SSL_ERROR_WANT_WRITE:
      assert(wantwrite);
      wantwrite(fd);
      break;
    default: break;
  }
  return inst->error;
}
#endif