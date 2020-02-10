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
    if(!h->connected) {
      ret = http_ssl_connect(h);
      if(http_ssl_io_again(h, ret) || ret == -1)
        return -1;
      if(!io_canwrite(h->sock))
        return http_ssl_io_errhandle(h, EWOULDBLOCK);
    }
    ret = http_ssl_write(h->sock, buf, len, b);
  } else
#endif
    ret = winsock2errno(send(fd, buf, len, 0));
  return ret;
}