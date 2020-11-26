#include "../ssl_internal.h"

#ifdef HAVE_OPENSSL
#include <openssl/ssl.h>
#include <openssl/err.h>

ssize_t
ssl_instance_return(ssl_instance* inst, int ret) {
  if(ret <= 0) {
    inst->error = SSL_get_error(inst->ssl, ret);
    if(inst->error == SSL_ERROR_WANT_WRITE) {
      if(inst->wantwrite)
        inst->wantwrite(SSL_get_fd(inst->ssl));
    } else if(inst->error == SSL_ERROR_WANT_READ) {
      if(inst->wantread)
        inst->wantread(SSL_get_fd(inst->ssl));
    }
  } else {
    inst->error = 0;
  }
  return ret;
}
#endif
