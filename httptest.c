#include "lib/windoze.h"
#include "lib/http.h"
#include "lib/strlist.h"
#include "lib/io.h"
#include "lib/iopause.h"
#include "lib/errmsg.h"
#include "lib/fmt.h"
#include "lib/byte.h"
#include "lib/open.h"
#include "lib/str.h"
#include "lib/unix.h"
#include "lib/slist.h"
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

typedef struct queue_entry_s {
  union {
    struct queue_entry_s* next;
    struct slink link;
  };
  uri_t uri;
  http* http;
} queue_entry;

static const char default_url[] = "https://www.google.com/                                  earch?q=SSL_bio";

static const char* const url_host = "127.0.0.1";
static const char* const url_location = "/login";
static const uint16 url_port = 8080;
static queue_entry* queue;

static io_entry* g_iofd;
static http h;
static buffer in, out;

static const char* token_types[] = {
    "XML_EOF",
    "XML_DATA",
    "XML_TAG_NAME",
    "XML_TAG_CLOSE",
    "XML_ATTR_NAME",
    "XML_ATTR_VALUE",
    "XML_COMMENT",
};

static const char* token_colors[] = {
    "\x1b[1;37m",
    "\x1b[1;31m",
    "\x1b[1;35m",
    "\x1b[1;33m",
    "\x1b[1;36m",
    "\x1b[1;32m",
};

static void
usage(char* av0) {
  buffer_putm_internal(buffer_1,
                       "Usage: ",
                       str_basename(av0),
                       " [OPTIONS] [FILES...]\n"
                       "\n"
                       "Options:\n"
                       "\n"
                       "  -h, --help              Show                        his help\n"
                       "  -o, --output FILE                              utput file\n"
                       "\n",
                       NULL);
  buffer_flush(buffer_1);
}

static queue_entry*
queue_put(void* head, const char* x) {
  queue_entry* e = alloc_zero(sizeof(queue_entry));

  uri_init(&e->uri);
  uri_scan(&e->uri, x, str_len(x));

  slist_push(head ? (slink**)head : (slink**)&queue, (slink*)e);

#ifdef DEBUG_OUTPUT_
  buffer_putspad(buffer_2, "queue_put", 12);
  buffer_puts(buffer_2, " len=");
  buffer_putulonglong(buffer_2, slist_length((slink**)&queue));
  buffer_puts(buffer_2, " x=");
  buffer_puts(buffer_2, x);
  buffer_putnlflush(buffer_2);
#endif

  return e;
}

static void
put_escaped_x(buffer* b, const char* x, size_t len, int unescaped) {
  for(size_t i = 0; i < len; i++) {
    char buf[32], c = x[i];

    if(c >= unescaped)
      buffer_putc(b, c);
    else
      buffer_put(b, buf, fmt_escapecharc(buf, (uint64)(unsigned char)c));
  }
}

static void
put_abbreviate(buffer* b, size_t len) {
  buffer_puts(b, "\n\033[38;5;230m... more (");
  buffer_putulong(b, len);
  buffer_puts(b, " bytes total ...\033[0m\n");
}

static void
put_escaped_n(buffer* b, const char* x, size_t len, size_t maxlen) {
  size_t n = len;

  if(n > maxlen)
    n = maxlen;

  put_escaped_x(b, x, n, 0x20);

  if(n < len) {
    put_abbreviate(b, len);
    n = (len - maxlen) >= maxlen ? len - maxlen : maxlen;
    put_escaped_x(b, &x[n], len - n, 0x20);
  }
}

static void
put_escaped(buffer* b, const char* x, size_t len) {
  put_escaped_x(b, x, len, 0x20);
}

static void
put_indented(buffer* b, const char* x, size_t len) {
  for(size_t i = 0; i < len; i++) {
    char c = x[i];
    buffer_putc(b, c);

    if(c == '\n')
      buffer_putc(b, '\t');
  }
}

/*static void
put_indented_n(buffer* b, const char* x, size_t len, size_t maxlen) {
  size_t n = len;

  if(n > maxlen)
    n = maxlen;

  put_indented(b, x, n);

  if(n < len) {
    put_abbreviate(b, len);
    n = (len - maxlen) >= maxlen ? len - maxlen : maxlen;
    put_indented(b, &x[n], len - n);
  }
}*/

static int
http_io_handler(http* h, buffer* out) {
  fd_type r, w;
  int nr = 0, nw = 0;
  ssize_t nb, ret = 0;

  while((w = io_canwrite()) != -1) {
    if(h->sock == w) {
      if((nb = http_canwrite(h, io_wantread, io_wantwrite)) <= 0) {
        ret = nb;
        continue;
      }

      if(nb > 0)
        ret += nb;

      nw++;
    }
  }

  while((r = io_canread()) != -1) {
    if(h->sock == r) {
  //  nb = http_canread(h, io_wantread, io_wantwrite);

      /*#ifdef DEBUG_OUTPUT
            buffer_putspad(buffer_2, "\x1b[1;31mhttp_canread\x1b[0m", 30);
            buffer_puts(buffer_2, "nb=");
            buffer_putlong(buffer_2, nb);
            buffer_puts(buffer_2, " connected=");
            buffer_putlong(buffer_2, !!h->connected);
            buffer_puts(buffer_2, " sent=");
            buffer_putlong(buffer_2, !!h->sent);

            if(nb < 0) {
              buffer_puts(buffer_2, " errno=");
              buffer_puts(buffer_2, unix_errno(errno));
            }

            buffer_putnlflush(buffer_2);
      #endif*/

      {
        char buf[8192];
        ssize_t len;

        if((len = buffer_get(&in, buf, sizeof(buf))) > 0) {
#ifdef DEBUG_OUTPUT
          buffer_putspad(buffer_2, "\x1b[1;31mbuffer_get\x1b[0m", 30);
          buffer_puts(buffer_2, " errno=");
          buffer_puts(buffer_2, unix_errno(errno));
          buffer_puts(buffer_2, " status=");
          buffer_puts(buffer_2,
                      ((const char* const[]){
                          "0", "HTTP_RECV_HEADER", "HTTP_RECV_DATA", "HTTP_STATUS_CLOSED", "HTTP_STATUS_ERROR", "HTTP_STATUS_BUSY", "HTTP_STATUS_FINISH", 0})[h->response->status]);
          buffer_puts(buffer_2, " len=");
          buffer_putlong(buffer_2, len);
          buffer_puts(buffer_2, " data='");
          put_escaped_n(buffer_2, buf, len, 20);
          buffer_putnlflush(buffer_2);
#endif

          if(buffer_put(out, buf, len)) {
            errmsg_warnsys("write error: ", 0);
            return 2;
          }

          if(len == -1 || h->response->status == HTTP_STATUS_ERROR) {
            errmsg_warnsys("read error: ", 0);
            return 1;
          }
        }

#ifdef DEBUG_OUTPUT
        buffer_puts(buffer_2, "h->response->status = ");
        buffer_putlong(buffer_2, h->response->status);
        buffer_putnlflush(buffer_2);
#endif

        return len;
      }

      ++nr;
    }
  }

fail:
  return ret;
}

static int
process_uris(const char* x, size_t len, strlist* urls, const uri_t* uri) {
  size_t i, pos = 0;
  uri_t link;
  uri_init(&link);

  while((i = pos + uri_find(&x[pos], len - pos)) < len) {
    size_t n = byte_chrs(&x[i], len - i, "\r\n\t\" ',", str_len("\r\n\t\" ',"));

    uri_copy(&link, uri);

    n = uri_scan(&link, &x[i], n);

#ifdef DEBUG_OUTPUT_
    buffer_puts(buffer_2, "uri: ");
    buffer_put(buffer_2, &x[i], n);
    buffer_puts(buffer_2, "\n");
    uri_dump(buffer_2, &link);

    buffer_putnlflush(buffer_2);
#endif
    strlist_pushb_unique(urls, &x[i], n);

    pos = i + n;
  }
}

static int
process_xml(const char* x, size_t len, strlist* urls, uri_t* uri) {
  xmlscanner s;
  xmltoken tok;
  static stralloc attr_name;

  xml_scanner(&s, x, len);

  do {
    tok = xml_read_token(&s);

    if(tok.id == XML_ATTR_NAME) {
      stralloc_copyb(&attr_name, tok.x, tok.len);
    } else

        if(tok.id == XML_ATTR_VALUE) {
      if(stralloc_equals(&attr_name, "href") || stralloc_equals(&attr_name, "src") || stralloc_equals(&attr_name, "url") || byte_finds(tok.x, tok.len, "://") < tok.len) {
        stralloc url;
        uri_t link;

        uri_init(&link);
        uri_copy(&link, uri);
        stralloc_init(&url);
        process_uris(tok.x + 1, tok.len - 2, urls, uri);
        // uri_scan(&link, tok.x + 1, tok.len - 2);

#ifdef DEBUG_OUTPUT_
        buffer_puts(buffer_2, "attr: ");
        buffer_putsa(buffer_2, &attr_name);

        buffer_puts(buffer_2, ", token: ");
        buffer_put(buffer_2, tok.x + 1, tok.len - 2);
        buffer_putnlflush(buffer_2);
#endif
        stralloc_free(&url);
      }
    } else if(tok.id == XML_DATA) {
      process_uris(tok.x, tok.len, urls, uri);
#ifdef DEBUG_OUTPUT_
      buffer_puts(buffer_2, tok.id < (sizeof(token_colors) / sizeof(token_colors[0])) ? token_colors[tok.id] : "?");
      buffer_putspad(buffer_2, token_types[tok.id + 1], 16);
      buffer_puts(buffer_2, "\"");
      buffer_put(buffer_2, tok.x, tok.len);
      buffer_puts(buffer_2, "\"\x1b[0m");
      buffer_putnlflush(buffer_2);
#endif
    }
  } while(tok.id != XML_EOF);
}

static void
http_process(http* h, strlist* urls, uri_t* uri) {
  http_response* r = h->response;

  const char* type = http_get_header(&h->response->data, "Content-Type");
  size_t typelen = type ? str_chrs(type, "\r\n\0", 3) : 0;

  size_t received = r->data.len;
  size_t pos = http_skip_header(stralloc_begin(&r->data), stralloc_length(&r->data));

  buffer_puts(buffer_2, "STATUS: ");
  buffer_puts(
      buffer_2,
      ((const char* const[]){"0", "HTTP_RECV_HEADER", "HTTP_RECV_DATA", "HTTP_STATUS_CLOSED", "HTTP_STATUS_ERROR", "HTTP_STATUS_BUSY", "HTTP_STATUS_FINISH", 0})[r->status]);

  buffer_putnlflush(buffer_2);
  buffer_puts(buffer_2, "PTR: ");
  buffer_putulong(buffer_2, r->ptr);
  buffer_putnlflush(buffer_2);
  buffer_puts(buffer_2, "TYPE: ");
  buffer_put(buffer_2, type, typelen);
  buffer_putnlflush(buffer_2);
  --pos;

  buffer_puts(buffer_2, "HEADERS: ");
  put_indented(buffer_2, stralloc_begin(&r->data), pos);
  buffer_puts(buffer_2, "RESPONSE LENGTH: ");
  buffer_putulonglong(buffer_2, r->data.len - r->ptr);
  buffer_puts(buffer_2, "\nRESPONSE DATA: ");

  if(0 && byte_finds(type, typelen, "html") < typelen || byte_finds(type, typelen, "xml") < typelen) {
    process_xml(stralloc_begin(&r->data) + pos, stralloc_length(&r->data) - pos, urls, uri);
  } else {
    put_escaped(buffer_2, stralloc_begin(&r->data) + pos, stralloc_length(&r->data) - pos);

    // put_indented_n(buffer_2, stralloc_begin(&r->data) + pos,
    // stralloc_length(&r->data) - pos, 1024);
  }

  buffer_putnlflush(buffer_2);
}

int
response_header(http* h, const char* x, size_t n) {

  buffer_puts(buffer_2, "response_header: ");
  buffer_put(buffer_2, x, n);
  buffer_putnlflush(buffer_2);
}

int
main(int argc, char* argv[]) {
  int argi;
  iopause_fd iop;
  static char inbuf[128 * 1024], outbuf[256 * 1024];
  fd_type fd, outfile;
  int c, index;
  buffer data;
  const char *s, *outname = 0;
  char* tmpl = "output-XXXXXX.txt";
  struct unix_longopt opts[] = {
      {"help", 0, NULL, 'h'},
      {"output", 0, NULL, 'o'},
      {0, 0, 0, 0},
  };

  errmsg_iam(argv[0]);
#if !WINDOWS_NATIVE
  // signal(SIGPIPE, SIG_IGN);
#endif
  // sig_block(SIGPIPE);
  tls_init(0, 0);
  // tls_new_client(0);

  for(;;) {
    c = unix_getopt_long(argc, argv, "ho:", opts, &index);

    if(c == -1)
      break;

    if(c == 0)
      continue;

    switch(c) {
      case 'o': outname = unix_optarg; break;
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

  buffer_init(&out, (buffer_op_proto*)(void*)&write, outfile, outbuf, sizeof(outbuf));
  buffer_init(&in, (buffer_op_proto*)(void*)&http_read, (uintptr_t)&h, inbuf, sizeof(inbuf));
  in.cookie = &h;

  http_init(&h, url_host, url_port);
  h.nonblocking = 1;
  h.keepalive = 1;
  h.version = 11;
  argi = unix_optind;

  if(argv[unix_optind] == 0) {
    argv[unix_optind++] = (char*)default_url;
    argv[unix_optind] = 0;
  }

  for(; argv[argi]; ++argi) {
    int ret;
    uri_t uri;
    char* str;
    strlist urls;

    strlist_init(&urls, '\0');
    uri_init(&uri);

    uri_scan(&uri, argv[argi], str_len(argv[argi]));

#ifdef DEBUG_OUTPUT
    uri_dump(buffer_2, &uri);
    buffer_putnlflush(buffer_2);
#endif

    // uri.port = 443;

    str = uri_str(&uri);

#ifdef DEBUG_OUTPUT
    buffer_puts(buffer_2, "URI: ");
    buffer_puts(buffer_2, str);
    buffer_putnlflush(buffer_2);
#endif

    free(str);
    ret = http_get(&h, argv[argi]);

    h.response->header = response_header;

    for(;;) {
      int doread = 1;
      fd_type sock;

#ifdef DEBUG_OUTPUT
      buffer_putsflush(buffer_2, "httptest start loop\n");
#endif

      io_wait();

      /*if(io_waituntil2(-1) == -1) {
        errmsg_warnsys("wait error: ", 0); return 3;
      }*/

      ret = http_io_handler(&h, &out);

      if(h.response->code == 302) {
        s = http_get_header(&h.response->data, "Location");
        buffer_puts(buffer_2, "Location: ");
        buffer_put(buffer_2, s, str_chrs(s, "\r\n", 2));
        buffer_putnlflush(buffer_2);
      }

#ifdef DEBUG_OUTPUT
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
                  ((const char* const[]){
                      "0", "HTTP_RECV_HEADER", "HTTP_RECV_DATA", "HTTP_STATUS_CLOSED", "HTTP_STATUS_ERROR", "HTTP_STATUS_BUSY", "HTTP_STATUS_FINISH", 0})[h.response->status]);
      buffer_putnlflush(buffer_2);
#endif

      // buffer_dump(buffer_1, &h.q.in);

      if(h.response->status == HTTP_STATUS_FINISH || h.response->status == HTTP_STATUS_CLOSED)
        break;
    }

    if(0) {
      const char *s = stralloc_begin(&h.response->data), *e = stralloc_end(&h.response->data);
      s += http_skip_header(s, e - s);

      put_escaped_n(buffer_1, s, e - s, 300);
      // buffer_put(buffer_1, s, e - s);
    }

    if(h.response->data.len) {
      const char* url;
      queue_entry** ptr = 0;

      // buffer_fromsa(&data, &h.response->data);
      http_process(&h, &urls, &uri);

      strlist_sort(&urls, 0);

      strlist_foreach_s(&urls, url) {
        queue_entry* entry = queue_put(ptr, url);
        ptr = &entry->next;
      }

#ifdef DEBUG_OUTPUT_
      strlist_dump(buffer_2, &urls);
      buffer_putnlflush(buffer_2);
#endif
    }

    buffer_flush(&out);
    buffer_putnlflush(buffer_1);
  }

  return 0;
}
