#define NO_BUILTINS
#include "../tls_internal.h"
#include "../http.h"
#include "../scan.h"
#include "../str.h"
#include "../stralloc.h"
#include "../buffer.h"
#include "../io.h"
#include "../byte.h"
#include "../errmsg.h"
#include "../socket.h"
#include "../fmt.h"
#include "../unix.h"
#include <errno.h>
#include <assert.h>
#include <string.h>

ssize_t
http_canwrite(http* h, void (*wantread)(fd_type), void (*wantwrite)(fd_type)) {
  ssize_t ret = 0;

  if(h->tls) {
    if(!h->connected) {
      ret = tls_connect(h->sock);

      if(ret == -1)
        tls_want(h->sock, wantread, wantwrite);

      if(ret != 1)
        goto fail;

      h->connected = 1;
    }
  } else {
    if(!h->connected)
      h->connected = 1;
  }

  if(h->connected && h->sent == 0) {
    ret = http_sendreq(h);

    if(ret == -1)
      if(h->tls)
        tls_want(h->sock, wantread, wantwrite);

    if(ret <= 0)
      goto fail;

    h->sent = 1;
  }

fail:
  /*if(ret == -1 && (errno == EAGAIN || errno == EWOULDBLOCK))
    tls_want(h->sock, wantread, wantwrite);*/
  h->err = h->tls ? tls_errno(h->sock) : errno;

#ifdef DEBUG_HTTP
  buffer_putspad(buffer_2, "\x1b[38;5;112mhttp_canwrite\x1b[0m", 30);
  buffer_puts(buffer_2, "ret=");
  buffer_putlong(buffer_2, ret);
  buffer_puts(buffer_2, " err=");
  buffer_putstr(buffer_2, http_strerror(h, ret));
  http_dump(h);
#endif

  return ret;
}
