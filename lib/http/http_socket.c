#include "../windoze.h"
//#define USE_WS2_32 1
#include "../socket_internal.h"
#include "../buffer.h"
#include "../http.h"
#include "../io.h"
#include "../ndelay.h"

#ifdef HAVE_OPENSSL
#include <openssl/ssl.h>
#include <openssl/err.h>
#endif

#if WINDOWS_NATIVE
#include <io.h>
#else
#include <unistd.h>
#endif
#include <errno.h>
#include <stdio.h>

size_t http_read_internal(http* h, char* buf, size_t n);


int
http_socket(http* h, int nonblock) {
  if((h->sock = socket_tcp4()) == -1)
    return -1;

  if(nonblock)
    ndelay_on(h->sock);
  else
    ndelay_off(h->sock);

  io_fd(h->sock);
#ifdef HAVE_OPENSSL
  if(h->tls)
    http_ssl_socket(h);

#endif

  if(h->q.in.x) {
    h->q.in.fd = h->sock;
  } else {
    buffer_read_fd(&h->q.in, h->sock);
    h->q.in.cookie = (void*)h;
  }

  h->q.in.op = (buffer_op_proto*)/*h->tls ? &http_ssl_read : */&http_socket_read;

  if(h->q.out.x) {
    h->q.out.fd = h->sock;
  } else {
    buffer_write_fd(&h->q.out, h->sock);
    h->q.out.cookie = (void*)h;
  }
  h->q.out.op = (buffer_op_proto*)(/*h->tls ? &http_ssl_write :*/ &http_socket_write);

  return 0;
}
