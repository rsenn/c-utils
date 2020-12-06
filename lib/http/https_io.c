#include "../http.h"
#include "../io.h"

#ifdef HAVE_OPENSSL
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <assert.h>
#include <errno.h>

const struct { int want_read, want_write, ssl; } https_error_code = {SSL_ERROR_WANT_READ, SSL_ERROR_WANT_WRITE, SSL_ERROR_SSL};
const struct { int eagain, ewouldblock; } https_errno_code = {EAGAIN, EWOULDBLOCK};
#endif
