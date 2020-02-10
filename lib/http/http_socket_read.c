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
http_socket_read(fd_t fd, void* buf, size_t len, void* b) {
  ssize_t ret;
  http* h = ((buffer*)b)->cookie;
  http_response* r = h->response;
  // s = winsock2errno(recv(fd, buf, len, 0));
#ifdef HAVE_OPENSSL
  if(h->ssl) {
    if(!h->connected) {
      ret = http_ssl_connect(h);
      if(http_ssl_io_again(h, ret) || ret == -1)
        return -1;
      if(!io_canread(h->sock))
        return http_ssl_io_errhandle(h, EAGAIN);
    }
    ret = http_ssl_read(h->sock, buf, len, b);
  } else
#endif
    ret = io_tryread(fd, buf, len);
  if(ret == 0) {
    closesocket(h->sock);
    h->q.in.fd = h->q.out.fd = h->sock = -1;
    r->status = HTTP_STATUS_CLOSED;
  } else if(ret == -1) {
    r->err = errno;
    if(errno != EWOULDBLOCK && errno != EAGAIN)
      r->status = HTTP_STATUS_ERROR;
    else
      return ret;
  }
  if(ret > 0) {
    size_t n = h->q.in.n;
    h->q.in.n += ret;
    ret = http_read_internal(h, buf, ret);
    h->q.in.n = n;
  }
  // putnum("http_socket_read", s);
  // putnum("err", r->err);
  return ret;
}