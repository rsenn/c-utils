#include "../http.h"
#include "../str.h"

#ifdef HAVE_OPENSSL
#include <openssl/ssl.h>
#include <openssl/err.h>

size_t
http_ssl_errstr(int errnum, char* buf, size_t buflen) {

  ERR_error_string_n(errnum, buf, buflen);
  ERR_clear_error();

  return str_len(buf);
}
#endif
