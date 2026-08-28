#include "../tls_internal.h"
#include "../buffer.h"
#include <assert.h>

#ifdef HAVE_OPENSSL
#include <openssl/ssl.h>
#include <openssl/err.h>

int
tls_want(fd_type fd, void (*wantread)(fd_type), void (*wantwrite)(fd_type)) {
  tls_instance_t* inst = tls_instance_get(fd);

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
#ifdef DEBUG_TLS
  buffer_putspad(buffer_2, "tls_want ", 30);
  buffer_puts(buffer_2, "fd=");
  buffer_putlong(buffer_2, fd);
  buffer_puts(buffer_2, " retval=");
  buffer_putlong(buffer_2, inst->retval);

  buffer_puts(buffer_2, " op=");
  buffer_puts(buffer_2, ((const char* const[]){"TLS_OP_WRITE", "TLS_OP_READ", "TLS_OP_ACCEPT", "TLS_OP_CONNECT", 0})[inst->op]);

  if(errno) {
    buffer_puts(buffer_2, " errno=");
    buffer_putstr(buffer_2, strerror(errno));
  }

  if(inst->error != SSL_ERROR_NONE) {

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
                                       "SSL_ERROR_WANT_CLIENT_HELLO_CB"})[inst->error]);
  }
  buffer_putnlflush(buffer_2);
#endif
  return inst->error;
}
#endif
