#include "../buffer.h"
#include "../http.h"
#include "../io.h"
#include "../socket.h"

int http_recv(int fd, void* buf, size_t len, buffer* b) {
  return recv(fd, buf, len, 0);
}

int http_send(int fd, const void* buf, size_t len, buffer* b) {
  return send(fd, buf, len, 0);
}

int
http_socket(http* h) {
	
  h->sock = socket_tcp4();
  io_nonblock(h->sock);

  if(h->q.in.x) {
	  h->q.in.fd = h->sock;
  } else  {
  buffer_read_fd(&h->q.in, h->sock);
  h->q.in.op = http_recv;
  }

  if(h->q.out.x) {
	  h->q.out.fd = h->sock;
  } else {
  buffer_write_fd(&h->q.out, h->sock);
  h->q.out.op = http_send;
  }

}
