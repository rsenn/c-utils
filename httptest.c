#include "lib/http.h"
#include "lib/buffer.h"
#include "lib/byte.h"
#include "lib/io_internal.h"
#include "lib/iopause.h"
#include "lib/socket.h"
#include "lib/taia.h"
#include "lib/errmsg.h"

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

/*
 *  URL: http://verteiler1.mediathekview.de/Filmliste-akt.xz
 */

static const char* const url_host = //"verteiler1.mediathekview.de";
    "www.fefe.de";                  //"5.1.76.111";
static const char* const url_location = "/gatling/";
static const uint16 url_port = 80;

int
main(int argc, char* argv[]) {

  http h;
  iopause_fd iop;
  static buffer in;
  char inbuf[8192];
  int ret;
  io_entry* e;
  fd_t fd;
  fd_t outfile = open_trunc("output.txt");

  errmsg_iam(argv[0]);

  http_init(&h, url_host, url_port);

  ret = http_get(&h, url_location);

  /*  buffer_init(&in, (buffer_op_sys*)&do_recv, h.sock, inbuf, sizeof(inbuf));

    buffer_puts(buffer_2, "http_get() = ");
    buffer_putlong(buffer_2, (long)ret);
    buffer_putnlflush(buffer_2);
  */
  io_fd(h.sock);

  e = io_getentry(h.sock);

  io_wantwrite(h.sock);
  /*

    byte_zero(&iop, sizeof(iop));
    iop.fd = h.sock;
    iop.events = IOPAUSE_WRITE;

    set_timeouts(10);
    iopause(&iop, 1, &deadline, &stamp);
  */

  for(;;) {
    int doread = 0;

    while((fd = io_canwrite()) != -1) {
      if(h.sock == fd) {
        http_sendreq(&h);
      }
    }

    if((doread = h.q.in.p < h.q.in.n) == 0) {

      while((fd = io_canread()) != -1) {
        if(h.sock == fd) {
          doread = 1;
        }
      }
    }

    if(doread) {
      char buf[1024];
      ssize_t n;
      if((n = buffer_get(&h.q.in, buf, sizeof(buf))) > 0)
        write(outfile, buf, n);
    }
    /*
    if(http_readable(&h, 1)) {
      ssize_t n = buffer_get(&h.q.in, buf, sizeof(buf));

      if(n > 0) {
        buffer_put(buffer_1, buf, n);
        buffer_flush(buffer_1);
      }
    }*/

    if(h.response->status == HTTP_STATUS_FINISH)
      break;

    io_wait();
  }

  /* buffer_putsa(buffer_1, &h.response->data);
   buffer_putnlflush(buffer_1);
  */
  return 0;
}
