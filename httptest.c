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
#include "lib/getopt.h"
#include <errno.h>
#include <signal.h>
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

void
usage(char* av0) {
  buffer_putm_internal(buffer_1,
                       "Usage: ",
                       str_basename(av0),
                       " [OPTIONS] [FILES...]\n"
                       "\n"
                       "Options:\n"
                       "\n"
                       "  -h, --help              Show this help\n"
                       "  -o, --output FILE       Output file\n"
                       "\n",
                       0);
  buffer_flush(buffer_1);
}

int
main(int argc, char* argv[]) {
  int argi;
  iopause_fd iop;
  static buffer in;
  static char inbuf[128 * 1024];
  static char outbuf[256 * 1024];
  fd_t fd, outfile;
  buffer out;
  int c, index;
  const char* outname = 0;
  const char* tmpl = "output-XXXXXX.txt";
  struct longopt opts[] = {{"help", 0, NULL, 'h'}, {"output", 0, NULL, 'o'}, {0, 0, 0, 0}};

  errmsg_iam(argv[0]);
  signal(SIGPIPE, SIG_IGN);

  for(;;) {
    c = getopt_long(argc, argv, "ho:", opts, &index);
    if(c == -1)
      break;
    if(c == 0)
      continue;

    switch(c) {
      case 'o': outname = optarg; break;
      case 'h': usage(argv[0]); return 0;

      default: usage(argv[0]); return 1;
    }
  }
  if(outname && str_equal(outname, "-"))
    outfile = 1;
  else if((outfile = outname ? open_trunc(outname) : open_temp(&tmpl)) == -1) {
    errmsg_warnsys("open error: ", outname, 0);
    return 126;
  }
  buffer_init(&out, &write, outfile, outbuf, sizeof(outbuf));
  http_init(&h, url_host, url_port);
  h.nonblocking = 1;
  h.keepalive = 0;
  argi = optind;
  if(argv[optind] == 0) {
    argv[optind++] = "https://127.0.0.1:443/show";
    // argv[1] = "http://127.0.0.1:5555/show";
    argv[optind] = 0;
  }
  for(; argi <= argc; ++argi) {
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
      //      buffer_dump(buffer_1, &h.q.in);
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