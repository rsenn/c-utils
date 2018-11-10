#include "lib/http.h"
#include "lib/buffer.h"
#include "lib/byte.h"
#include "lib/io.h"
#include "lib/iopause.h"
#include "lib/socket.h"
#include "lib/taia.h"

#include <errno.h>
#ifdef __ORANGEC__
#include <sockets.h>
#endif

static int last_errno = 0;

static struct taia deadline, stamp;

static void
set_timeouts(int seconds) {
  taia_uint(&deadline, seconds);
  taia_uint(&stamp, 0);
}

static ssize_t
do_recv(int s, void* buf, size_t len, void* ptr) {
  ssize_t ret = recv(s, buf, len, 0);
  if(ret == -1) {
    last_errno = errno;
    errno = 0;
    /*    if(errno == EAGAIN) {
          errno = 0;
          ret = -1;
        }*/
  }
  return ret;
}

/*
 *  URL: http://verteiler1.mediathekview.de/Filmliste-akt.xz
 */

static const char* const url_host = //"verteiler1.mediathekview.de";
 "5.1.76.111";
static const char* const url_location = "/Filmliste-akt.xz";
static const uint16 url_port = 80;

int
main(int argc, char* argv[]) {

  http h;
  iopause_fd iop;
  static buffer in;
  char inbuf[8192];
  int ret;

  errmsg_iam(argv[0]);

  http_init(&h, url_host, url_port);

  ret = http_get(&h, url_location);

  buffer_init(&in, (buffer_op_sys*)&do_recv, h.sock, inbuf, sizeof(inbuf));

  buffer_puts(buffer_2, "http_get() = ");
  buffer_putlong(buffer_2, (long)ret);
  buffer_putnlflush(buffer_2);

  byte_zero(&iop, sizeof(iop));
  iop.fd = h.sock;
  iop.events = /*IOPAUSE_READ|*/ IOPAUSE_WRITE;

  set_timeouts(10);
  iopause(&iop, 1, &deadline, &stamp);

  http_sendreq(&h);

  for(;;) {
    iop.events = IOPAUSE_READ;

    set_timeouts(10);
    iopause(&iop, 1, &deadline, &stamp);

    if(iop.revents & IOPAUSE_READ) {
      http_readable(&h);

      if(h.response->status == HTTP_STATUS_FINISH) break;
    }
  }

  buffer_putsa(buffer_1, &h.response->data);
  buffer_putnlflush(buffer_1);

  return 0;
}
