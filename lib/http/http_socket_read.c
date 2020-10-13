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
  ssize_t ret = -1;
  http* h = (http*)((buffer*)b)->cookie;
  http_response* r = h->response;
#if DEBUG_OUTPUT_
  buffer_putsflush(buffer_2, "http_socket_read\n");
#endif
// s = winsock2errno(recv(fd, buf, len, 0));
#ifdef HAVE_OPENSSL
  if(h->ssl) {
    if(!h->connected) {
      ret = http_ssl_connect(h);
      if(h->connected) {
        if((ret = http_sendreq(h)) > 0)
          ret = 0;
      }
    }
    if(h->connected || ret == 0)
      ret = http_ssl_read(h->sock, buf, len, b);
  } else
#endif
    ret = io_tryread(fd, (char*)buf, len);
  if(ret == 0) {
    // closesocket(h->sock);
    http_close(h);
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
    ret = http_read_internal(h, (char*)buf, ret);
    h->q.in.n = n;
  }
#if DEBUG_OUTPUT_
  buffer_puts(buffer_2, "http_socket_read ");
  buffer_puts(buffer_2, " ret=");
  buffer_putlong(buffer_2, ret);
  if(ret <= 0) {
    buffer_puts(buffer_2, " errno=");
    buffer_putlong(buffer_2, errno);
  }
  buffer_putnlflush(buffer_2);
#endif

  return ret;
}
