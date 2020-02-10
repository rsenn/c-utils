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
  if(h->tls) {
    http_ssl_socket(h);

    buffer_putsflush(buffer_2, "ssl socket\n");
  }
#endif

  buffer_init_free(&h->q.in, &http_socket_read, h->sock, malloc(BUFFER_INSIZE), BUFFER_INSIZE);
  h->q.in.cookie = (void*)h;

  buffer_init_free(&h->q.out, &http_socket_write, h->sock, malloc(BUFFER_OUTSIZE), BUFFER_OUTSIZE);
  h->q.out.cookie = (void*)h;

  return 0;
}
