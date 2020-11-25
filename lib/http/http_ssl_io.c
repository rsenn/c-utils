#include "../http.h"
#include "../io.h"

#ifdef HAVE_OPENSSL
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <assert.h>
#include <errno.h>

const struct { int want_read, want_write, ssl; } http_ssl_error_code = {SSL_ERROR_WANT_READ, SSL_ERROR_WANT_WRITE, SSL_ERROR_SSL};

const struct { int eagain, ewouldblock; } http_ssl_errno_code = {EAGAIN, EWOULDBLOCK};
/*
ssize_t
http_ssl_io_want(http* h, int err) {
 int ret;
 switch(err) {
   case SSL_ERROR_WANT_READ: {
     io_dontwantwrite(h->sock);
     io_wantread(h->sock);
     ret = EAGAIN;
     break;
   }
   case SSL_ERROR_WANT_WRITE: {
     io_dontwantread(h->sock);
     io_wantwrite(h->sock);
     ret = EWOULDBLOCK;
     break;
   }
   default: {
     ret = errno;
     break;
   }
 }
 return ret;
}
*/
#endif