#include "../typedefs.h"
#include "../windoze.h"
#include "../alloc.h"
#include "../socket_internal.h"
#include "../uint64.h"
#include "../buffer.h"
#include "../http.h"
#include "../io.h"
#include "../ndelay.h"
#include "../tls.h"
#include "../byte.h"

#if WINDOWS_NATIVE
#include <io.h>
#else
#include <unistd.h>
#endif
#include <errno.h>
#include <string.h>
#include <stdio.h>

ssize_t http_read_internal(fd_t fd, char* buf, size_t received, buffer* b);

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

  buffer_init_free(&h->q.in, (buffer_op_sys*)(void*)&http_socket_read, h->sock, h->q.in.x ? h->q.in.x : (char*)alloc(BUFFER_INSIZE), h->q.in.a ? h->q.in.a : BUFFER_INSIZE);
  h->q.in.cookie = (void*)h;
  buffer_init_free(&h->q.out, (buffer_op_sys*)(void*)&http_socket_write, h->sock, h->q.out.x ? h->q.out.x : (char*)alloc(BUFFER_OUTSIZE), h->q.out.a ? h->q.out.a : BUFFER_OUTSIZE);
  h->q.out.cookie = (void*)h;

#if DEBUG_HTTP
  buffer_putspad(buffer_2, "http_socket", 30);
  buffer_puts(buffer_2, "h->s=");
  buffer_putlonglong(buffer_2, h->sock);

  buffer_putnlflush(buffer_2);
#endif
  return h->sock;
}

ssize_t
http_socket_read(fd_t fd, void* buf, size_t len, void* b) {
  ssize_t ret = -1, iret = -1;
  http* h = (http*)((buffer*)b)->cookie;
  http_response* r = h->response;
  int connected = h->connected;
  int tlserr;

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

  tlserr = h->tls ? tls_error(h->sock) : 0;

  if(h->tls && ret < 0 && tlserr) {
    h->err = tls_errno(h->sock);
    ret = -1;
  } else if(ret == 0 || (h->tls && tlserr == TLS_ERR_SYSCALL)) {
    h->connected = 0;
    r->status = HTTP_STATUS_CLOSED;
    ret = 0;
  } else if(ret == -1 && (!h->tls || tlserr == TLS_ERR_ZERO_RETURN)) {
    r->err = errno;
    if(h->tls ? (tlserr != TLS_ERR_WANT_READ && tlserr != TLS_ERR_WANT_WRITE) : ( r->err != EWOULDBLOCK &&  r->err != EAGAIN))
      r->status = HTTP_STATUS_ERROR;
  }
#if DEBUG_HTTP
  buffer_putspad(buffer_2, "http_socket_read", 30);
  buffer_puts(buffer_2, "s=");
  buffer_putlong(buffer_2, h->sock);
  buffer_puts(buffer_2, " len=");
  buffer_putlong(buffer_2, len);
  buffer_puts(buffer_2, " ret=");
  buffer_putlong(buffer_2, ret);
  if(ret < 0) {
    buffer_puts(buffer_2, " errno=");
    buffer_putstr(buffer_2, strerror(errno));
  }

  if(h->response) {
    buffer_puts(buffer_2, " transfer=");
    buffer_puts(buffer_2, "HTTP_TRANSFER_");
    buffer_puts(buffer_2, ((const char* const[]){"UNDEF", "CHUNKED", "LENGTH", "BOUNDARY", 0})[h->response->transfer]);

    buffer_puts(buffer_2, " status=");
    buffer_puts(buffer_2,
                ((const char* const[]){
                    "-1", "HTTP_RECV_HEADER", "HTTP_RECV_DATA", "HTTP_STATUS_CLOSED", "HTTP_STATUS_ERROR", "HTTP_STATUS_BUSY", "HTTP_STATUS_FINISH", 0})[h->response->status + 1]);
  }
  buffer_putnlflush(buffer_2);
#endif
  if(ret > 0) {
    int st = h->response->status;
    size_t n = h->q.in.n;
    /*   buffer_realloc(&h->q.in, h->q.in.n + ret);

       byte_copy(&h->q.in.x[h->q.in.n], ret, buf);*/
    if(st == HTTP_RECV_HEADER) {
      buffer_realloc(&h->q.in, h->q.in.n + ret);

      byte_copy(&h->q.in.x[h->q.in.n], ret, buf);
      h->q.in.n += ret;
    }

    iret = http_read_internal(fd, (char*)buf, ret, &h->q.in);
    if(st == HTTP_RECV_HEADER)
      h->q.in.n = n;
  }
  if(ret == 0) {
    io_dontwantwrite(fd);
    io_dontwantread(fd);
  }

  return ret;
}

ssize_t
http_socket_write(fd_t fd, void* buf, size_t len, void* b) {
  http* h = (http*)((buffer*)b)->cookie;
  http_response* r = h->response;
  ssize_t ret = 0;
  int tlserr, connected = h->connected;

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

  tlserr = h->tls ? tls_error(h->sock) : 0;

  if(h->tls && ret < 0 && tlserr == 2) {
    errno = EAGAIN;
    ret = -1;
  } else if(h->tls && ret < 0 && tlserr == 3) {
    errno = EWOULDBLOCK;
    ret = -1;
  } else if(ret == 0 || (h->tls && tlserr == 5)) {
    h->connected = 0;
    r->status = HTTP_STATUS_CLOSED;
    ret = 0;
  } else if(ret == -1 && (!h->tls || tlserr != 5)) {
    r->err = errno;
    if(h->tls ? (tlserr != 2 && tlserr != 3) : (errno != EWOULDBLOCK && errno != EAGAIN))
      r->status = HTTP_STATUS_ERROR;
  }
#ifdef DEBUG_HTTP
  buffer_putspad(buffer_2, "http_socket_write ", 30);
  buffer_puts(buffer_2, "s=");
  buffer_putlong(buffer_2, h->sock);

  buffer_puts(buffer_2, " ret=");
  buffer_putlong(buffer_2, ret);
  if(ret < 0) {
    buffer_puts(buffer_2, " errno=");
    buffer_putstr(buffer_2, strerror(errno));
  }

  if(h->response) {
    buffer_puts(buffer_2, " transfer=");
    buffer_puts(buffer_2, "HTTP_TRANSFER_");
    buffer_puts(buffer_2, ((const char* const[]){"UNDEF", "CHUNKED", "LENGTH", "BOUNDARY", 0})[h->response->transfer]);

    buffer_puts(buffer_2, " status=");
    buffer_puts(buffer_2,
                ((const char* const[]){
                    "-1", "HTTP_RECV_HEADER", "HTTP_RECV_DATA", "HTTP_STATUS_CLOSED", "HTTP_STATUS_ERROR", "HTTP_STATUS_BUSY", "HTTP_STATUS_FINISH", 0})[h->response->status + 1]);
  }
  buffer_putnlflush(buffer_2);
#endif
  return ret;
}
