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
  if(ret <= 0) {
    i->error = SSL_get_error(i->ssl, ret);
    if(i->error == SSL_ERROR_WANT_WRITE) {
#ifdef DEBUG_OUTPUT_
      tls_instance_debug(i);
      buffer_putsflush(buffer_2, " wants write\n");
#endif
      if(i->wantwrite) {
        BIO_get_fd(SSL_get_wbio(i->ssl), &fd);
        i->wantwrite(fd);
      }
    } else if(i->error == SSL_ERROR_WANT_READ) {
#ifdef DEBUG_OUTPUT_
      tls_instance_debug(i);
      buffer_putsflush(buffer_2, " wants read\n");
#endif
      if(i->wantread) {
        BIO_get_fd(SSL_get_rbio(i->ssl), &fd);

        i->wantread(fd);
      }
    }
  }
  return ret;
}
#endif
