#include "../http.h"
#include "../scan.h"
#include "../socket_internal.h"
#include "../str.h"
#include "../stralloc.h"
#include "../io.h"
#include "../byte.h"
#include <errno.h>
#include <assert.h>

ssize_t
http_socket_write(fd_t fd, void* buf, size_t len, void* b) {
  http* h = ((buffer*)b)->cookie;
  ssize_t ret = 0;
#ifdef HAVE_OPENSSL
  if(h->ssl) {
    assert(h->connected);
    ret = http_ssl_write(h->sock, buf, len, b);
  }
#endif

#ifdef HAVE_OPENSSL
  if(!h->ssl)
    ret = winsock2errno(send(fd, buf, len, 0));
#endif
  return ret;
}