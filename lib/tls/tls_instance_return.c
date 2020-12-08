#include "../tls_internal.h"
#include "../buffer.h"

#ifdef HAVE_OPENSSL
#include <openssl/ssl.h>
#include <openssl/err.h>

ssize_t
tls_instance_return(tls_instance* i, tls_op_t op, int ret) {
  fd_t fd = 0;
  i->op = op;
  i->error = 0;
  i->retval = ret;

  /* if(ret <= 0)*/ {
    i->error = SSL_get_error(i->ssl, ret);
    if(i->error == SSL_ERROR_WANT_WRITE) {
      /*   if(i->wantwrite) {
           BIO_get_fd(SSL_get_wbio(i->ssl), &fd);
           i->wantwrite(fd);
         }*/
    } else if(i->error == SSL_ERROR_WANT_READ) {
      /*  if(i->wantread) {
          BIO_get_fd(SSL_get_rbio(i->ssl), &fd);
          i->wantread(fd);
        }*/
    }
  }
  return ret;
}
#endif
