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
#if DEBUG_HTTP
  buffer_putspad(buffer_2, "http_socket_read ", 18);
  buffer_puts(buffer_2, "sock=");
  buffer_putlong(buffer_2, h->sock);
  buffer_putnlflush(buffer_2);
#endif
#ifdef HAVE_OPENSSL
  if(h->ssl) {
    /* if(!h->connected) {
       ret = https_connect(h);*/
    /*   if(h->connected) {
         if((ret = http_sendreq(h)) > 0)
           ret = 0;
       }
     }*/
    // if(h->connected || ret == 0)
    ret = tls_read(h->sock, buf, len);
  } else
#endif
    ret = io_tryread(fd, (char*)buf, len);
  if(ret == 0) {
    http_close(h);
    h->q.in.fd = h->q.out.fd = h->sock = -1;
    h->connected = 0;
    r->status = HTTP_STATUS_CLOSED;
  } else if(ret == -1) {
    r->err = errno;
    if(errno != EWOULDBLOCK && errno != EAGAIN)
      r->status = HTTP_STATUS_ERROR;
  }
  if(ret > 0) {
    size_t n = h->q.in.n;
    h->q.in.n += ret;
    ret = http_read_internal(h, (char*)buf, ret);
    h->q.in.n = n;
  }
#if DEBUG_HTTP
  buffer_putspad(buffer_2, "http_socket_read ", 18);
  buffer_puts(buffer_2, "sock=");
  buffer_putlong(buffer_2, h->sock);
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
