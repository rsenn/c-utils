#include "../tls_internal.h"
#include "../tls_internal.h"
#include "../stralloc.h"
#include "../buffer.h"
#include "../fmt.h"
#include "../uint64.h"
#include <assert.h>
#include <errno.h>

#ifdef HAVE_OPENSSL
#include <openssl/ssl.h>
#include <openssl/err.h>

ssize_t
tls_read(fd_t fd, void* data, size_t len) {
  int64 ret;
  tls_instance* i = iarray_get(&tls_list, fd);
  assert(i);
  assert(i->ssl);

  if(!SSL_is_init_finished(i->ssl)) {
    if((ret = tls_instance_handshake(i)) != 1) {
      if(ret < 0)
        errno = tls_instance_errno(i);
      return ret;
    }
  }

  if((tls_instance_return(i, TLS_OP_READ, ret = SSL_read(i->ssl, data, len))) < 0)
    errno = tls_instance_errno(i);

#ifdef DEBUG_TLS
  buffer_putspad(buffer_2, "tls_read ", 30);
  buffer_puts(buffer_2, "fd: ");
  buffer_putlong(buffer_2, fd);
  buffer_puts(buffer_2, " ret: ");
  buffer_putlonglong(buffer_2, ret);
  if(errno) {
    buffer_puts(buffer_2, " errno: ");
    buffer_putstr(buffer_2, strerror(errno));
  }
  if(i->error != SSL_ERROR_NONE) {
    buffer_puts(buffer_2, " error: ");
    buffer_puts(buffer_2, ((const char* const[]){"SSL_ERROR_NONE", "SSL_ERROR_SSL", "SSL_ERROR_WANT_READ", "SSL_ERROR_WANT_WRITE", "SSL_ERROR_WANT_X509_LOOKUP", "SSL_ERROR_SYSCALL", "SSL_ERROR_ZERO_RETURN", "SSL_ERROR_WANT_CONNECT", "SSL_ERROR_WANT_ACCEPT", "SSL_ERROR_WANT_ASYNC", "SSL_ERROR_WANT_ASYNC", "SSL_ERROR_WANT_ASYNC_JOB", "SSL_ERROR_WANT_CLIENT_HELLO_CB"})[i->error]);
  }
  if(ret > 0) {
    size_t len = ret;
    buffer_puts(buffer_2,
                "\n"
                "                              "
                "data: \"");

    if(len > 16)
      len = 16;
    buffer_put_escaped(buffer_2, data, len, &fmt_escapecharnonprintable);
    buffer_puts(buffer_2, "\"");
    if(len < ret) {
      buffer_puts(buffer_2, "... more (");

      buffer_putulong(buffer_2, ret);
      buffer_puts(buffer_2, " bytes total) ...");
    }
  }
  buffer_putnlflush(buffer_2);
#endif
  return ret;
}
#endif
