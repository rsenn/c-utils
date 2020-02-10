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
    err = http_ssl_error(h, ret);
    if(err == SSL_ERROR_SSL)
      return 1;
    else if(http_ssl_io(h, err) < -1)
      return -1;
  }
  if(ret == 1) {
    if(!h->connected) {
      h->connected = 1;
#if DEBUG_OUTPUT
      buffer_putsflush(buffer_2, "connected\n");
#endif
    }
    if(h->connected)
      io_wantwrite(h->sock);
  }
  return ret;
}
#endif