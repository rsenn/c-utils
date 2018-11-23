#include "../windoze.h"
//#define USE_WS2_32 1
#include "../socket_internal.h"
#include "../buffer.h"
#include "../http.h"
#include "../io.h"

#if WINDOWS_NATIVE
#include <io.h>
#else
#include <unistd.h>
#endif
#include <errno.h>
size_t http_read_internal(http* h, char* buf, size_t n);

ssize_t http_socket_read(fd_t fd, void* buf, size_t len, buffer* b);

static ssize_t
http_socket_write(fd_t fd, void* buf, size_t len, buffer* b) {
  http* h = b->cookie;

  return winsock2errno(send(fd, buf, len, 0));
}

int
http_socket(http* h) {

  h->sock = socket_tcp4();
  io_nonblock(h->sock);

  if(h->q.in.x) {
    h->q.in.fd = h->sock;
  } else {
    buffer_read_fd(&h->q.in, h->sock);
    h->q.in.cookie = (void*)h;
  }
  h->q.in.op = (buffer_op_proto*)&http_socket_read;

  if(h->q.out.x) {
    h->q.out.fd = h->sock;
  } else {
    buffer_write_fd(&h->q.out, h->sock);
    h->q.out.cookie = (void*)h;
  }
  h->q.out.op = (buffer_op_proto*)&http_socket_write;
}
