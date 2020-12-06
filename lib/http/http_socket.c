#include "../typedefs.h"
#include "../windoze.h"
#include "../alloc.h"
//#define USE_WS2_32 1
#include "../socket_internal.h"
#include "../buffer.h"
#include "../http.h"
#include "../io.h"
#include "../ndelay.h"
#include "../tls.h"

#if WINDOWS_NATIVE
#include <io.h>
#else
#include <unistd.h>
#endif
#include <errno.h>
#include <string.h>
#include <stdio.h>

ssize_t http_socket_read(fd_t fd, void* buf, size_t len, void* b);
ssize_t http_socket_write(fd_t fd, void* buf, size_t len, void* b);

int
http_socket(http* h, int nonblock) {
  if((h->sock = socket_tcp4()) == -1)
    return -1;

  if(h->tls) {
    if((h->ssl = tls_client(h->sock)) == 0)
      return -1;
    tls_io(h->sock);
  }
  io_fd(h->sock);
  if(nonblock)
    ndelay_on(h->sock);

#if DEBUG_HTTP
  buffer_putsflush(buffer_2, "ssl socket\n");
#endif

  buffer_init_free(&h->q.in,
                   (buffer_op_sys*)(void*)&http_socket_read,
                   h->sock,
                   (char*)alloc(BUFFER_INSIZE),
                   BUFFER_INSIZE);
  h->q.in.cookie = (void*)h;
  buffer_init_free(&h->q.out,
                   (buffer_op_sys*)(void*)&http_socket_write,
                   h->sock,
                   (char*)alloc(BUFFER_OUTSIZE),
                   BUFFER_OUTSIZE);
  h->q.out.cookie = (void*)h;
  return 0;
}

ssize_t
http_socket_read(fd_t fd, void* buf, size_t len, void* b) {
  ssize_t ret = -1;
  http* h = (http*)((buffer*)b)->cookie;
  http_response* r = h->response;
  int connected = h->connected;

  if(h->tls) {

    ret = tls_read(h->sock, buf, len);
    if(!connected && tls_established(h->sock))
      h->connected = 1;
  } else {
    if(!connected)
      h->connected = 1;
    ret = io_tryread(fd, (char*)buf, len);
  }
  if(!connected && h->connected) {
    //    if(h->response->status <= HTTP_RECV_HEADER)
    h->response->status = HTTP_RECV_HEADER;
  }

  if(ret == 0) {
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
    ret = http_read_internal(fd, (char*)buf, ret, &h->q.in);
    h->q.in.n = n;
  }
  if(ret == 0) {
    io_dontwantwrite(fd);
    io_dontwantread(fd);
  }
#if DEBUG_HTTP
  buffer_putspad(buffer_2, "http_socket_read", 30);
  buffer_puts(buffer_2, "sock=");
  buffer_putlong(buffer_2, h->sock);
  buffer_puts(buffer_2, " ret=");
  buffer_putlong(buffer_2, ret);
  if(ret < 0) {
    buffer_puts(buffer_2, " errno=");
    buffer_putstr(buffer_2, strerror(errno));
  }
  buffer_putnlflush(buffer_2);
#endif
  return ret;
}

ssize_t
http_socket_write(fd_t fd, void* buf, size_t len, void* b) {
  http* h = (http*)((buffer*)b)->cookie;
  ssize_t ret = 0;
  int connected = h->connected;

  if(h->tls) {
    ret = tls_write(h->sock, buf, len);
    if(!connected && tls_established(h->sock))
      h->connected = 1;
  } else {
    if(!connected)
      h->connected = 1;

    ret = winsock2errno(send(fd, buf, len, 0));
  }
  if(!connected && h->connected) {
    //    if(h->response->status <= HTTP_RECV_HEADER)
    h->response->status = HTTP_RECV_HEADER;
  }

#ifdef DEBUG_HTTP
  buffer_putspad(buffer_2, "http_socket_write ", 30);
  buffer_puts(buffer_2, "sock=");
  buffer_putlong(buffer_2, h->sock);

  buffer_puts(buffer_2, " ret=");
  buffer_putlong(buffer_2, ret);
  if(ret < 0) {
    buffer_puts(buffer_2, " errno=");
    buffer_putstr(buffer_2, strerror(errno));
  }
  buffer_putnlflush(buffer_2);
#endif
  return ret;
}
