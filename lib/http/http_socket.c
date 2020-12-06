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
#include <stdio.h>

size_t http_read_internal(http* h, char* buf, size_t n);

int
http_socket(http* h, int nonblock) {
  if((h->sock = socket_tcp4()) == -1)
    return -1;

  if(h->tls) {
    if((h->ssl = tls_client(h->sock)) == 0)
      return -1;
  }
  io_fd(h->sock);
  if(nonblock)
    ndelay_on(h->sock);

#if DEBUG_HTTP
  buffer_putsflush(buffer_2, "ssl socket\n");
#endif

  buffer_init_free(&h->q.in, (buffer_op_sys*)(void*)&http_socket_read, h->sock, (char*)alloc(BUFFER_INSIZE), BUFFER_INSIZE);
  h->q.in.cookie = (void*)h;
  buffer_init_free(&h->q.out, (buffer_op_sys*)(void*)&http_socket_write, h->sock, (char*)alloc(BUFFER_OUTSIZE), BUFFER_OUTSIZE);
  h->q.out.cookie = (void*)h;
  return 0;
}
