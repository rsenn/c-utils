#include "../tls_internal.h"
#include "../buffer.h"

#ifdef HAVE_OPENSSL
#include <openssl/ssl.h>
#include <openssl/err.h>

ssize_t
tls_instance_return(tls_instance_t* i, tls_op_t op, int ret) {
  fd_t fd = 0;
  i->op = op;
  i->error = 0;
  i->retval = ret;

  /* if(ret <= 0)*/ {
    i->error = SSL_get_error(i->ssl, ret);
  }
  return ret;
}
#endif
