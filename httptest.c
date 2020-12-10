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
#include "lib/tls.h"
#include "lib/sig.h"
#include "lib/xml.h"

#include "uri.h"

#include <errno.h>
#include <string.h>
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
 *
 *  URL: http://verteiler1.mediathekview.de/Filmliste-akt.xz
 */

/* https://github.com/rsenn/lc-meter/raw/master/doc/LCmeter0-LCD-8pinlcd-PIC_COMP.pdf */
static const char default_url[] =
    "https://www.google.com/search?q=SSL_bio"; //"https://raw.githubusercontent.com/rsenn/lc-meter/master/doc/LCmeter0-LCD-8pinlcd-PIC_COMP.pdf";
static const char* const url_host = "127.0.0.1";
static const char* const url_location = "/login";
static const uint16 url_port = 8080;
static io_entry* g_iofd;
static http h;
static buffer in, out;

const char* token_types[] = {"XML_EOF",
                             "XML_DATA",
                             "XML_TAG_NAME",
                             "XML_TAG_CLOSE",
                             "XML_ATTR_NAME",
                             "XML_ATTR_VALUE",
                             "XML_COMMENT"};
const char* token_colors[] = {
    "\x1b[1;37m", "\x1b[1;31m", "\x1b[1;35m", "\x1b[1;33m", "\x1b[1;36m", "\x1b[1;32m"};

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
                       NULL);
  buffer_flush(buffer_1);
}

static void
put_escaped_x(buffer* b, const char* x, size_t len, int unescaped) {
  size_t i;
  char buf[32];

  for(i = 0; i < len; i++) {
    char c = x[i];

    if(c >= unescaped) {
      buffer_putc(b, c);
    } else {
      /*buffer_putc(b, '\\'); */
      buffer_put(b, buf, fmt_escapecharc(buf, (uint64)(unsigned char)c));
    }
  }
}

static void
put_escaped(buffer* b, const char* x, size_t len) {
  put_escaped_x(b, x, len, 0x20);
}

static int
http_io_handler(http* h, buffer* out) {
  fd_t r, w;
  int nr = 0, nw = 0;
  ssize_t nb, ret = 0;
  while((w = io_canwrite()) != -1) {
    if(h->sock == w) {
      if((nb = http_canwrite(h, io_wantread)) <= 0) {
        ret = nb;
        continue;
      }

      if(nb > 0)
        ret += nb;
      nw++;
    }
  }

  while((r = io_canread()) != -1) {
#ifdef DEBUG_OUTPUT
    buffer_putspad(buffer_2, "io_canread", 30);
    buffer_puts(buffer_2, "r=");
    buffer_putlong(buffer_2, r);

    buffer_putnlflush(buffer_2);
#endif
    if(h->sock == r) {
      nb = http_canread(h, io_wantwrite);

#ifdef DEBUG_OUTPUT
      buffer_putspad(buffer_2, "\x1b[1;31mhttp_io_handler canread\x1b[0m", 30);
      buffer_puts(buffer_2, "nb=");
      buffer_putlong(buffer_2, nb);
      buffer_puts(buffer_2, " connected=");
      buffer_putlong(buffer_2, !!h->connected);
      buffer_puts(buffer_2, " sent=");
      buffer_putlong(buffer_2, !!h->sent);
      if(nb < 0) {
        buffer_puts(buffer_2, "  errno=");
        buffer_puts(buffer_2, strerror(errno));
      }
      buffer_putnlflush(buffer_2);
#endif
      /* if(nb <= 0) {
         ret = nb;
         continue;
       }
       if(nb > 0)
         ret += nb;
       if(h->connected && h->sent)*/
      {
        char buf[8192];
        ssize_t len;

        if((len = buffer_get(&in, buf, sizeof(buf))) > 0) {
          buffer_putspad(buffer_2, "\x1b[1;31mbuffer_get\x1b[0m", 30);
          buffer_puts(buffer_2, " errno=");
          buffer_puts(buffer_2, strerror(errno));
          buffer_puts(buffer_2, " status=");
          buffer_puts(buffer_2,
                      ((const char* const[]){"-1",
                                             "HTTP_RECV_HEADER",
                                             "HTTP_RECV_DATA",
                                             "HTTP_STATUS_CLOSED",
                                             "HTTP_STATUS_ERROR",
                                             "HTTP_STATUS_BUSY",
                                             "HTTP_STATUS_FINISH",
                                             0})[h->response->status + 1]);
          buffer_puts(buffer_2, " len=");
          buffer_putlong(buffer_2, len);
          buffer_puts(buffer_2, " data='");
          put_escaped_x(buffer_2, buf, len, 0x40);
          buffer_putnlflush(buffer_2);

          if(buffer_put(out, buf, len)) {
            errmsg_warnsys("write error: ", 0);
            return 2;
          }
          if(len == -1 || h->response->status == HTTP_STATUS_ERROR) {
            errmsg_warnsys("read error: ", 0);
            return 1;
          }
        }

        buffer_puts(buffer_2, "h->response->status = ");
        buffer_putlong(buffer_2, h->response->status);
        buffer_putnlflush(buffer_2);
        return len;
      }

      nr++;
    }
  }
fail:
  return ret;
}

int
process_xml(buffer* data) {
  xmlscanner s;
  xmltoken tok;

  xml_scanner(&s, data);

  do {
    tok = xml_read_token(&s);

    buffer_puts(buffer_2, token_colors[tok.id]);
    buffer_putspad(buffer_2, token_types[tok.id + 1], 16);
    put_escaped_x(buffer_2, tok.x, tok.len, 0x40);
    buffer_puts(buffer_2, "\x1b[0m");
    /*  buffer_puts(buffer_2, "\nXML token length = ");
          buffer_putulong(buffer_2, tok.len);*/
    buffer_putnlflush(buffer_2);
  } while(tok.id != XML_EOF);
}

void
http_dump(http* h) {

  const char* type = http_get_header(h, "Content-Type");
  buffer_puts(buffer_2, "ptr: ");
  buffer_putulong(buffer_2, h->response->ptr);
  buffer_putnlflush(buffer_2);
  buffer_puts(buffer_2, "type: ");
  buffer_put(buffer_2, type, str_chrs(type, "\r\n\0", 3));
  buffer_putnlflush(buffer_2);

  buffer_puts(buffer_2, "response: ");
  put_escaped_x(buffer_2, h->response->data.s, h->response->data.len, 0x20);
  buffer_putnlflush(buffer_2);
}

int
main(int argc, char* argv[]) {
  int argi;
  iopause_fd iop;
  static char inbuf[128 * 1024];
  static char outbuf[256 * 1024];
  fd_t fd, outfile;
  int c, index;
  buffer data;
  const char *s, *outname = 0;
  char* tmpl = "output-XXXXXX.txt";
  struct longopt opts[] = {{"help", 0, NULL, 'h'}, {"output", 0, NULL, 'o'}, {0, 0, 0, 0}};

  errmsg_iam(argv[0]);
#if !WINDOWS_NATIVE
  // signal(SIGPIPE, SIG_IGN);
#endif
  // sig_block(SIGPIPE);
  tls_init(0, 0);
  // tls_new_client(0);

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
  buffer_init(&out, (buffer_op_sys*)(void*)&write, outfile, outbuf, sizeof(outbuf));
  buffer_init(&in, (buffer_op_sys*)(void*)&http_read, (uintptr_t)&h, inbuf, sizeof(inbuf));
  in.cookie = &h;

  http_init(&h, url_host, url_port);
  h.nonblocking = 1;
  h.keepalive = 0;
  h.version = 10;
  argi = optind;
  if(argv[optind] == 0) {
    argv[optind++] = (char*)default_url;
    // argv[1] = "http://127.0.0.1:5555/show";
    argv[optind] = 0;
  }
  for(; argv[argi]; ++argi) {
    int ret;
    uri_t uri;
    char* str;
    uri_init(&uri);

    uri_scan(argv[argi], &uri);
    uri_dump(buffer_2, &uri);

    // uri.port = 443;

    str = uri_str(&uri);

    buffer_puts(buffer_2, "URI: ");
    buffer_puts(buffer_2, str);
    buffer_putnlflush(buffer_2);
    free(str);
    ret = http_get(&h, argv[argi]);
    for(;;) {

      int doread = 1;
      fd_t sock;
      ;
      buffer_putsflush(buffer_2, "httptest start loop\n");

      io_wait();

      /*if(io_waituntil2(-1) == -1) {
        errmsg_warnsys("wait error: ", 0);
        return 3;
      }*/

      ret = http_io_handler(&h, &out);

      if(h.response->code == 302) {
        s = http_get_header(&h, "Location");
        buffer_puts(buffer_2, "Location: ");
        buffer_put(buffer_2, s, str_chrs(s, "\r\n", 2));
        buffer_putnlflush(buffer_2);
      }

      buffer_putspad(buffer_2, "httptest", 30);
      buffer_puts(buffer_2, "ret=");
      buffer_putlong(buffer_2, ret);

      buffer_puts(buffer_2, " err=");
      buffer_puts(buffer_2, http_strerror(&h, ret));
      if(h.response->code != -1) {
        buffer_puts(buffer_2, " code=");
        buffer_putlong(buffer_2, h.response->code);
      }

      buffer_puts(buffer_2, " status=");
      buffer_puts(buffer_2,
                  ((const char* const[]){"-1",
                                         "HTTP_RECV_HEADER",
                                         "HTTP_RECV_DATA",
                                         "HTTP_STATUS_CLOSED",
                                         "HTTP_STATUS_ERROR",
                                         "HTTP_STATUS_BUSY",
                                         "HTTP_STATUS_FINISH",
                                         0})[h.response->status + 1]);
      buffer_putnlflush(buffer_2);

      // buffer_dump(buffer_1, &h.q.in);
      if(h.response->data.len) {
      }
      if(h.response->status == HTTP_STATUS_FINISH || h.response->status == HTTP_STATUS_CLOSED)
        break;
    }
    buffer_put(buffer_1, stralloc_begin(&h.response->data), h.response->ptr);
    //   put_escaped_x(buffer_1, stralloc_begin(&h.response->data),
    //   stralloc_length(&h.response->data), 0x12);

    buffer_fromsa(&data, &h.response->data);

    http_dump(&h);
    //  process_xml(&data);

    // buffer_putsa(&out, &h.response->data);
    buffer_flush(&out);
    /* buffer_putsa(buffer_1, &h.response->data);*/
    buffer_putnlflush(buffer_1);
  }
  return 0;
}
