#include "lib/http.h"
#include "lib/byte.h"
#include "lib/io_internal.h"
#include "lib/iopause.h"
#include "lib/socket.h"
#include "lib/taia.h"
#include "lib/errmsg.h"
#include "lib/open.h"
#include "lib/dns.h"
#include "lib/fmt.h"
#include "lib/iarray.h"
#include "lib/scan.h"
#include "lib/str.h"
#include "lib/stralloc.h"
#include "lib/tai.h"
#include "lib/case.h"
#include "lib/buffer.h"

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
  fd_t fd, outfile;
  const char* outname = "output-XXXXXX.txt";

  errmsg_iam(argv[0]);

//  unlink(outname);
  if((outfile = open_temp(outname)) == -1) {
    errmsg_warnsys("open error: ", 0);
    return 126;
  }

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

      if(io_waituntil2(-1) == -1) {
        errmsg_warnsys("wait error: ", 0);
        return 3;
      }

      while((fd = io_canwrite()) != -1) {
        if(h.sock != fd)
          continue;

        if(http_sendreq(&h) == -1) {
          errmsg_warnsys("send error: ", 0);
          return 2;
        }
      }

      while((fd = io_canread()) != -1) {
        if(h.sock == fd) {
          doread = 1;
        }
      }

      while((n = http_read(&h, buf, sizeof(buf))) > 0) {
        if(write(outfile, buf, n) == -1) {
          errmsg_warnsys("write error: ", 0);
          return 2;
        }

        if(n == -1 || h.response->status == HTTP_STATUS_ERROR) {
          errmsg_warnsys("read error: ", 0);
          return 1;
        }
      }

      buffer_dump(buffer_1, &h.q.in);

      if(h.response->status >= HTTP_STATUS_CLOSED)
        break;
    }

    /* buffer_putsa(buffer_1, &h.response->data);*/
    buffer_putnlflush(buffer_1);
  }

  return 0;
}