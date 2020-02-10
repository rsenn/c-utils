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
      if((ret = http_ssl_connect(h->sock, h)) == -1)
        return -1;
      if(h->connected)
        buffer_putsflush(buffer_2, "ssl handshake done\n");
    }

    if(!io_canread(h->sock) && !io_canwrite(h->sock)) {
      errno = EAGAIN;
      return -1;
    }

    ret = http_ssl_read(h->sock, buf, len, b);

  } else
#endif
  {
    ret = io_tryread(fd, buf, len);
  }

  /*  buffer_puts(buffer_2, "io_tryread(");
    buffer_putlong(buffer_2, fd);
    buffer_puts(buffer_2, ", ");
    buffer_putptr(buffer_2, buf);
    buffer_puts(buffer_2, ", ");
    buffer_putulong(buffer_2, len);
    buffer_puts(buffer_2, ") = ");
    buffer_putlong(buffer_2, s);
    buffer_putnlflush(buffer_2);*/
  if(ret == 0) {
    closesocket(h->sock);
    h->q.in.fd = h->q.out.fd = h->sock = -1;
    r->status = HTTP_STATUS_CLOSED;
  } else if(ret == -1) {
    r->err = errno;
    if(errno != EWOULDBLOCK && errno != EAGAIN) {
      r->status = HTTP_STATUS_ERROR;
    } else {
      errno = 0;
    }
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