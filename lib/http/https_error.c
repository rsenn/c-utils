#include "../http.h"
#include "../io.h"

#ifdef HAVE_OPENSSL
#include <openssl/ssl.h>
#include <openssl/err.h>

const char*
https_errflag(int i) {
  switch(i) {
    case SSL_ERROR_NONE: return "NONE";
    case SSL_ERROR_SSL: return "SSL";
    case SSL_ERROR_WANT_READ: return "WANT_READ";
    case SSL_ERROR_WANT_WRITE: return "WANT_WRITE";
    case SSL_ERROR_WANT_X509_LOOKUP: return "WANT_X509_LOOKUP";
    case SSL_ERROR_SYSCALL: return "SYSCALL";
    case SSL_ERROR_ZERO_RETURN: return "ZERO_RETURN";
    case SSL_ERROR_WANT_CONNECT: return "WANT_CONNECT";
    case SSL_ERROR_WANT_ACCEPT: return "WANT_ACCEPT";
    case SSL_ERROR_WANT_ASYNC: return "WANT_ASYNC";
    case SSL_ERROR_WANT_ASYNC_JOB: return "WANT_ASYNC_JOB";
    case SSL_ERROR_WANT_CLIENT_HELLO_CB: return "WANT_CLIENT_HELLO_CB";
    default: return "no such error";
  }
}

#endif
