#include "../http.h"
#include "../io.h"
#include "../socket.h"
#include "../windoze.h"
#include "../tls.h"
#if WINDOWS_NATIVE
#include <io.h>
#else
#include <unistd.h>
#include <netdb.h>
#endif

void
http_close(http* h) {
  ssize_t ret;

  if(h->tls) {
    ret = tls_close(h->sock);
  } else {
    socket_close(h->sock);
    ret = 1;
  }

  if(ret == 1) {
    io_close(h->sock);

    h->q.in.fd = h->q.out.fd = h->sock = -1;
  }

  if(ret == -1 && (errno == EAGAIN || errno == EWOULDBLOCK))
    tls_want(h->sock, io_wantread, io_wantwrite);

#ifdef DEBUG_HTTP
  buffer_putspad(buffer_2, "http_close ", 18);
  buffer_puts(buffer_2, "sock=");
  buffer_putlong(buffer_2, h->sock);
  buffer_puts(buffer_2, " ret=");
  buffer_putlong(buffer_2, ret);
  buffer_puts(buffer_2, " err=");
  buffer_puts(buffer_2, http_strerror(h, ret));
  buffer_putnlflush(buffer_2);
#endif
}
