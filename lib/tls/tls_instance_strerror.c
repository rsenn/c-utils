#include "../tls_internal.h"
#include "../buffer.h"
#include "../str.h"

#ifdef HAVE_OPENSSL
#include <openssl/ssl.h>
#include <openssl/err.h>

const char*
tls_instance_strerror(tls_instance_t* i) {
  switch(i->error) {
    case SSL_ERROR_WANT_READ: str_copy(i->errstr, "want read"); break;
    case SSL_ERROR_WANT_WRITE: str_copy(i->errstr, "want write"); break;
    case SSL_ERROR_WANT_CONNECT: str_copy(i->errstr, "want connect"); break;
    case SSL_ERROR_WANT_ACCEPT: str_copy(i->errstr, "want accept"); break;
    case SSL_ERROR_WANT_ASYNC: str_copy(i->errstr, "want async"); break;
    case SSL_ERROR_SYSCALL:
      str_copyn(i->errstr, "syscall ", sizeof(i->errstr));
      str_catn(i->errstr, strerror(errno), sizeof(i->errstr));
      break;
    case SSL_ERROR_ZERO_RETURN: str_copy(i->errstr, "zero return"); break;
    case SSL_ERROR_NONE: str_copy(i->errstr, "none"); break;
    default: ERR_error_string_n(i->error, i->errstr, sizeof(i->errstr)); break;
  }
  return i->errstr;
}
#endif
