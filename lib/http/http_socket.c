#define NO_BUILTINS
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
#include "../unix.h"

#if WINDOWS_NATIVE
#include <io.h>
#else
#include <unistd.h>
#endif
#include <errno.h>
#include <string.h>
#include <stdio.h>

#define HTTP_RECV_BUFSIZE 16384
#define HTTP_SEND_BUFSIZE 32768

int
http_socket(http* h, int nonblock) {
  buffer *in = &h->q.in, *out = &h->q.out;

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

  buffer_init_free(in, &http_socket_read, h->sock, in->x ? in->x : (char*)alloc(HTTP_RECV_BUFSIZE), in->a ? in->a : HTTP_RECV_BUFSIZE);
  in->cookie = (void*)h;

  buffer_init_free(out, &http_socket_write, h->sock, out->x ? out->x : (char*)alloc(HTTP_SEND_BUFSIZE), out->a ? out->a : HTTP_SEND_BUFSIZE);
  out->cookie = (void*)h;

#ifdef DEBUG_HTTP
  buffer_putspad(buffer_2, "http_socket", 30);
  buffer_puts(buffer_2, "h->s=");
  buffer_putlonglong(buffer_2, h->sock);
  buffer_putnlflush(buffer_2);
#endif

  return h->sock;
}

ssize_t
http_socket_read(fd_type fd, void* buf, size_t len, buffer* b) {
  http* h = b->cookie;
  http_response* response = h->response;
  int err = 0, tlserr = 0, connected = h->connected;
  ssize_t ret = -1, iret = -1;

  if(h->tls) {
    ret = tls_read(h->sock, buf, len);

    if(ret < 0)
      tlserr = h->tls ? tls_error(h->sock) : 0;

    if(!connected && tls_established(h->sock))
      h->connected = 1;
  } else {
    if(!connected)
      h->connected = 1;

    ret = recv(fd, buf, len, 0);

    if(ret < 0)
      err = errno;

    if(ret == -1) {
      buffer_puts(buffer_2, "failed recv()");
      buffer_putnlflush(buffer_2);
    }
  }

#if 1 // def DEBUG_OUTPUT
  buffer_putspad(buffer_2, "\x1b[38;5;197mhttp_socket_read\x1b[0m", 30);
  buffer_puts(buffer_2, "recv() = ");
  buffer_putlong(buffer_2, ret);

  if(ret < 0) {
    buffer_puts(buffer_2, ", errno=");
    buffer_puts(buffer_2, unix_errno(err));
  }

  buffer_putnlflush(buffer_2);
#endif

  if(!response->status && h->connected)
    response->status = HTTP_RECV_HEADER;

  if(h->tls && ret < 0 && tlserr) {
    h->err = tls_errno(h->sock);
    ret = -1;
  }

  if(ret == 0 || (h->tls && tlserr == TLS_ERR_ZERO_RETURN)) {
    h->connected = 0;
    response->status = HTTP_STATUS_CLOSED;
    ret = 0;
  } else if(ret == -1 && (!h->tls || tlserr == TLS_ERR_SYSCALL)) {
    response->err = err;

    if(h->tls ? (tlserr != TLS_ERR_WANT_READ && tlserr != TLS_ERR_WANT_WRITE) : (response->err != EWOULDBLOCK && response->err != EAGAIN))
      response->status = HTTP_STATUS_ERROR;
  }

#ifdef DEBUG_HTTP_
  buffer_putspad(buffer_2, "\x1b[38;5;64mhttp_socket_read\x1b[0m", 30);
  buffer_puts(buffer_2, " ret=");
  buffer_putlong(buffer_2, ret);
  buffer_puts(buffer_2, " len=");
  buffer_putlong(buffer_2, len);

  if(errno) {
    buffer_puts(buffer_2, " errno=");
    buffer_puts(buffer_2, unix_errno(errno));
  }

  http_dump(h);
#endif

  /*if(ret > 0) {
    int st = response->status;
    size_t n = h->q.in.n;
    if(st == HTTP_RECV_HEADER) {
      buffer_realloc(in, h->q.in.n + ret);
      byte_copy(in.x[h->q.in.n], ret, buf);
      h->q.in.n += ret;
    }

  i
    if(st == HTTP_RECV_HEADER)
      h->q.in.n = n;
  }*/
  //  iret = http_read_internal(fd, (char*)buf, ret, in);

  if(response->status == HTTP_STATUS_CLOSED || response->status == HTTP_STATUS_ERROR) {
    io_dontwantwrite(fd);
    io_dontwantread(fd);
  }

  return ret;
}

ssize_t
http_socket_write(fd_type fd, void* buf, size_t len, buffer* b) {
  http* h = b->cookie;
  http_response* response = h->response;
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

  if(!response->status && h->connected)
    response->status = HTTP_RECV_HEADER;

  tlserr = h->tls ? tls_error(h->sock) : 0;

  if(h->tls && ret < 0 && tlserr == 2) {
    errno = EAGAIN;
    ret = -1;
  } else if(h->tls && ret < 0 && tlserr == 3) {
    errno = EWOULDBLOCK;
    ret = -1;
  } else if(ret == 0 || (h->tls && tlserr == 5)) {
    h->connected = 0;
    response->status = HTTP_STATUS_CLOSED;
    ret = 0;
  } else if(ret == -1 && (!h->tls || tlserr != 5)) {
    response->err = errno;

    if(h->tls ? (tlserr != 2 && tlserr != 3) : (errno != EWOULDBLOCK && errno != EAGAIN))
      response->status = HTTP_STATUS_ERROR;
  }

#ifdef DEBUG_HTTP_
  buffer_putspad(buffer_2, "\x1b[38;5;88mhttp_socket_write\x1b[0m ", 30);
  buffer_puts(buffer_2, "s=");
  buffer_putlong(buffer_2, h->sock);
  buffer_puts(buffer_2, " ret=");
  buffer_putlong(buffer_2, ret);

  if(ret < 0) {
    buffer_puts(buffer_2, " errno=");
    buffer_puts(buffer_2, unix_errno(errno));
  }

  http_dump(h);
#endif

  if(response->status == HTTP_STATUS_CLOSED || response->status == HTTP_STATUS_ERROR) {
    io_dontwantwrite(fd);
    io_dontwantread(fd);
  }

  return ret;
}
