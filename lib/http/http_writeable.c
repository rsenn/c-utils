#include "../http.h"
#include "../scan.h"
#include "../socket_internal.h"
#include "../str.h"
#include "../stralloc.h"
#include "../io.h"
#include "../byte.h"
#include <errno.h>
#include <assert.h>

#ifdef HAVE_OPENSSL
#include <openssl/ssl.h>
#include <openssl/err.h>
#endif

ssize_t
http_writeable(http* h) {
  ssize_t ret = 0;
#ifdef HAVE_OPENSSL
  if(h->ssl) {
    if(!h->connected) {
      if((ret = http_ssl_connect(h->sock, h)) == 1) {
        h->connected = 1;
        h->connected = 1;
        io_wantwrite(h->sock);

      } else if(ret == -1) {
        errno = EWOULDBLOCK;
      }
      errmsg_infosys("writeable: SSL handshake = ",
                     ret == 1 ? "done" : ret == 0 ? "eof" : errno == 0 ? "success" : "error",
                     0);
      return ret;
    }
  }
#endif
request:
  h->connected = 1;
  if(h->connected) {
    http_sendreq(h);
    io_dontwantwrite(h->sock);
    io_wantread(h->sock);
  }
}
