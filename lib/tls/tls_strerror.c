#include "../tls_internal.h"
#include "../buffer.h"
#include "../str.h"
#include "../unix.h"
#include <errno.h>
#include <string.h>

#ifdef HAVE_OPENSSL
#include <openssl/ssl.h>
#include <openssl/err.h>

const char*
tls_strerror(fd_type fd) {
  int err;
  const char* str;
  tls_instance_t* i = iarray_get(&tls_list, fd);

  switch(i->error) {
    case SSL_ERROR_WANT_READ: str_copy(i->errstr, "want read"); break;
    case SSL_ERROR_WANT_WRITE: str_copy(i->errstr, "want write"); break;
    case SSL_ERROR_WANT_CONNECT: str_copy(i->errstr, "want connect"); break;
    case SSL_ERROR_WANT_ACCEPT: str_copy(i->errstr, "want accept"); break;
#ifdef SSL_ERROR_WANT_ASYNC
    case SSL_ERROR_WANT_ASYNC: str_copy(i->errstr, "want async"); break;
#endif
    case SSL_ERROR_SYSCALL:
      str_copyn(i->errstr, "syscall ", sizeof(i->errstr));
      str_catn(i->errstr, unix_errno(i->syserr), sizeof(i->errstr));
      break;
    case SSL_ERROR_ZERO_RETURN: str_copy(i->errstr, "zero return"); break;
    case SSL_ERROR_NONE: str_copy(i->errstr, "none"); break;
    default: ERR_error_string_n(i->error, i->errstr, sizeof(i->errstr)); break;
  }
  return i->errstr;
}
#endif
