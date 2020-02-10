#include "../http.h"

#ifdef HAVE_OPENSSL
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <assert.h>

ssize_t
http_ssl_read(fd_t fd, void* buf, size_t len, void* b) {
  http* h = ((buffer*)b)->cookie;
  char* msg = 0;
  ssize_t ret = 0;
  errno = 0;
  if(!h->tls)
    return 0;
  /* if(!h->connected) {
     if((ret = http_ssl_connect(fd, h)) == 1)
       h->connected = 1;
     if(h->connected) {
       io_wantwrite(fd);
       if(!io_canread(fd)) {
         errno = EAGAIN;
         return -1;
       }
     }
     return ret;
   }*/
  assert(h->connected);
do_read:
  if((ret = SSL_read(h->ssl, buf, len)) <= 0) {
    buffer_puts(buffer_2, "http_ssl_read ");
    ret = http_ssl_error(ret, h, 0);
  }
  return ret;
}
#endif