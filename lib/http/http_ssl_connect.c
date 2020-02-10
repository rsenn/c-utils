#include "../http.h"
#include "../io.h"

#ifdef HAVE_OPENSSL
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <assert.h>

ssize_t
http_ssl_connect(http* h) {
  ssize_t ret;
  char* msg = 0;
  errno = 0;
  assert(!h->connected);
  ret = SSL_connect(h->ssl);
  if(ret <= 0) {
    int err = http_ssl_error(h, ret);

    if(err)
      ret = http_ssl_io(h, err);
  }

  if(ret == 1) {
    if(!h->connected) {
      h->connected = 1;
      buffer_putsflush(buffer_2, "connected\n");
    }
     io_wantwrite(h->sock);
  }
  return ret;
}
#endif