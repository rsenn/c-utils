#include "../http.h"
#include "../io.h"

#ifdef HAVE_OPENSSL
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <assert.h>

ssize_t
http_ssl_connect(http* h) {
  ssize_t ret;
  int err = 0;
  char* msg = 0;
  errno = 0;
  assert(!h->connected);
  if((ret = SSL_connect(h->ssl)) <= 0) {
    if(http_ssl_io_again(h, ret))
      return -1;
    if((err = http_ssl_error(h, ret)) == SSL_ERROR_SSL) {
      ret = 1;
      err = 0;
    }
    if(err)
      return http_ssl_io_errhandle(h, err);
  }
  if(ret == 1) {
    h->connected = 1;
    io_wantwrite(h->sock);
#if DEBUG_OUTPUT
    buffer_putsflush(buffer_2, "http_ssl_connect\n");
#endif
  }
  return ret;
}
#endif