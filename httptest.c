#include "lib/windoze.h"
#include "lib/http.h"
#include "lib/byte.h"
#include "lib/io.h"
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
#if WINDOWS_NATIVE
#include <io.h>
#else
#include <unistd.h>
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
static const char* const url_host = "127.0.0.1";
static const char* const url_location = "/login";
static const uint16 url_port = 8080;
static io_entry* g_iofd;
static http h;
int
main(int argc, char* argv[]) {
  int argi;
  iopause_fd iop;
  static buffer in;
  static char inbuf[128 * 1024];
  static char outbuf[256 * 1024];
  fd_t fd, outfile;
  buffer out;
  const char* outname = "output-XXXXXX.txt";
  errmsg_iam(argv[0]);
  //  unlink(outname);
  if((outfile = open_temp(&outname)) == -1) {
    errmsg_warnsys("open error: ", 0);
    return 126;
  }
  buffer_init(&out, &write, outfile, outbuf, sizeof(outbuf));
  http_init(&h, url_host, url_port);
  h.nonblocking = 1;
  h.keepalive = 0;
  if(argc < 2) {
    argv[1] = "https://127.0.0.1:443/show";
    // argv[1] = "http://127.0.0.1:5555/show";
    argc++;
  }
  for(argi = 1; argi < argc; ++argi) {
    int ret = http_get(&h, argv[argi]);
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
        http_writeable(&h);
      }
      while((fd = io_canread()) != -1) {
        if(h.sock == fd) {
          doread = 1;
        }
      }
      if(doread)
        http_readable(&h, 1);
      h.q.in.cookie = &h;
      while((n = http_read(&h, buf, sizeof(buf), &h.q.in)) > 0) {
        if(buffer_put(&out, buf, n)) {
          errmsg_warnsys("write error: ", 0);
          return 2;
        }
        if(n == -1 || h.response->status == HTTP_STATUS_ERROR) {
          errmsg_warnsys("read error: ", 0);
          return 1;
        }
      }
      buffer_dump(buffer_1, &h.q.in);
      if(h.response->status >= HTTP_STATUS_CLOSED) {
        break;
      }
    }
    buffer_flush(&out);
    /* buffer_putsa(buffer_1, &h.response->data);*/
    buffer_putnlflush(buffer_1);
  }
  return 0;
}