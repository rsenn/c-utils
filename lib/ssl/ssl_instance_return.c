#include "../ssl_internal.h"
#include "../buffer.h"

#ifdef HAVE_OPENSSL
#include <openssl/ssl.h>
#include <openssl/err.h>

ssize_t
ssl_instance_return(ssl_instance* i, int ret) {
  if(ret <= 0) {
    i->error = SSL_get_error(i->ssl, ret);
    if(i->error == SSL_ERROR_WANT_WRITE) {
#ifdef DEBUG_OUTPUT
      buffer_puts(buffer_2, "ssl instance ");
      buffer_putlong(buffer_2, SSL_get_fd(i->ssl));
      buffer_putsflush(buffer_2, " wants write\n");
#endif
      if(i->wantwrite)
        i->wantwrite(SSL_get_fd(i->ssl));
    } else if(i->error == SSL_ERROR_WANT_READ) {
#ifdef DEBUG_OUTPUT
      buffer_puts(buffer_2, "ssl instance ");
      buffer_putlong(buffer_2, SSL_get_fd(i->ssl));
      buffer_putsflush(buffer_2, " wants read\n");
#endif
      if(i->wantread)
        i->wantread(SSL_get_fd(i->ssl));
    }
  } else {
    i->error = 0;
  }
  return ret;
}
#endif
