#include "../tls_internal.h"
#include "../stralloc.h"
#include "../buffer.h"
#include "../fmt.h"
#include <assert.h>

#ifdef HAVE_OPENSSL
#include <openssl/ssl.h>
#include <openssl/err.h>

ssize_t
tls_write(fd_type fd, const void* data, size_t len) {
  ssize_t ret;
  tls_instance_t* i = iarray_get(&tls_list, fd);

  assert(i);
  assert(i->ssl);

  if(!SSL_is_init_finished(i->ssl)) {
    if((ret = tls_instance_handshake(i)) != 1) {
      if(ret < 0)
        errno = tls_instance_errno(i);

      return ret;
    }
  }

  if((ret = tls_instance_return(i, TLS_OP_WRITE, SSL_write(i->ssl, data, len))) < 0)
    errno = tls_instance_errno(i);

#ifdef DEBUG_TLS
  buffer_putspad(buffer_2, "tls_write ", 30);
  buffer_puts(buffer_2, "fd=");
  buffer_putlong(buffer_2, fd);
  buffer_puts(buffer_2, " ret=");
  buffer_putlong(buffer_2, ret);

  if(errno) {
    buffer_puts(buffer_2, " errno=");
    buffer_putstr(buffer_2, strerror(errno));
  }

  buffer_puts(buffer_2, " retval=");
  buffer_putlong(buffer_2, i->retval);

  if(i->error != SSL_ERROR_NONE) {
    buffer_puts(buffer_2, " error=");
    buffer_puts(buffer_2,
                ((const char* const[]){"SSL_ERROR_NONE",
                                       "SSL_ERROR_SSL",
                                       "SSL_ERROR_WANT_READ",
                                       "SSL_ERROR_WANT_WRITE",
                                       "SSL_ERROR_WANT_X509_LOOKUP",
                                       "SSL_ERROR_SYSCALL",
                                       "SSL_ERROR_ZERO_RETURN",
                                       "SSL_ERROR_WANT_CONNECT",
                                       "SSL_ERROR_WANT_ACCEPT",
                                       "SSL_ERROR_WANT_ASYNC",
                                       "SSL_ERROR_WANT_ASYNC",
                                       "SSL_ERROR_WANT_ASYNC_JOB",
                                       "SSL_ERROR_WANT_CLIENT_HELLO_CB"})[i->error]);
  }

  if(ret > 0) {
    buffer_puts(buffer_2, " data=");
    buffer_putfmt(buffer_2, data, ret, &fmt_escapecharnonprintable);
  }

  buffer_putnlflush(buffer_2);
#endif

  return ret;
}
#endif
