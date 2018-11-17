#include "lib/http.h"
#include "lib/buffer.h"
#include "lib/byte.h"
#include "lib/io_internal.h"
#include "lib/iopause.h"
#include "lib/socket.h"
#include "lib/taia.h"
#include "lib/errmsg.h"
#include "lib/open.h"

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
static io_entry* g_iofd;
static http h;

int
main(int argc, char* argv[]) {
  int argi;
  iopause_fd iop;
  static buffer in;
  char inbuf[8192];
  int ret;
  fd_t fd;
  fd_t outfile = open_trunc("output.txt");

  errmsg_iam(argv[0]);

  http_init(&h, url_host, url_port);

  if(argc < 2) {
    argv[1] = "www.fefe.de/gatling/";
    argc++;
  }

  for(argi = 1; argi < argc; ++argi) {

    ret = http_get(&h, argv[argi]);

    /*  buffer_init(&in, (buffer_op_sys*)&do_recv, h.sock, inbuf, sizeof(inbuf));

      buffer_puts(buffer_1, "http_get() = ");
      buffer_putlong(buffer_1, (long)ret);
      buffer_putnlflush(buffer_1);
    */
    io_fd(h.sock);

    g_iofd = io_getentry(h.sock);

    io_wantwrite(h.sock);
    io_wantread(h.sock);
    /*

      byte_zero(&iop, sizeof(iop));
      iop.fd = h.sock;
      iop.events = IOPAUSE_WRITE;

      set_timeouts(10);
      iopause(&iop, 1, &deadline, &stamp);
    */

    for(;;) {
      char buf[8192];
      ssize_t n;
      int doread = 0;

      io_wait();

      while((fd = io_canwrite()) != -1) {
        if(h.sock == fd) {
          http_sendreq(&h);
        }
      }

      while((fd = io_canread()) != -1) {
        if(h.sock == fd) {
          doread = 1;
        }
      }

      if(!doread)
        doread = h.q.in.p < h.q.in.n;

      if(doread) {
        while((n = http_read(&h, buf, sizeof(buf))) > 0) {
          write(outfile, buf, n);
          //    write(1, buf, n);
          /*j
                    buffer_put(buffer_1, "Wrote ", 7);
                    buffer_putlong(buffer_1, n);
                    buffer_put(buffer_1, " bytes...", 9);
                    buffer_putnlflush(buffer_1);*/
        }
        buffer_dump(buffer_1, &h.q.in);
      }
      /*
      if(http_readable(&h, 1)) {
        ssize_t n = buffer_get(&h.q.in, buf, sizeof(buf));

        if(n > 0) {
          buffer_put(buffer_1, buf, n);
          buffer_flush(buffer_1);
        }
      }*/

      if(h.response->status >= HTTP_STATUS_CLOSED)
        break;
    }

    /* buffer_putsa(buffer_1, &h.response->data);*/
    buffer_putnlflush(buffer_1);
  }

  return 0;
}
