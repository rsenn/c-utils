#include "../../lib/alloc.h"
#include "../../lib/buffer.h"
#include "../../lib/case.h"
#include "../../lib/errmsg.h"
#include "../../lib/http.h"
#include "../../lib/scan.h"
#include "../../lib/str.h"
#include "../../lib/stralloc.h"
#include "../../lib/tls.h"
#include "../../lib/uri.h"
#include "../../lib/unix.h"
#include "../dnsutils/address.h"

#include <sys/stat.h>
#include <unistd.h>

typedef struct {
  int fd, tls;
  buffer in, out;
  char inbuf[16384], outbuf[16384];
} conn;

static int show_headers, follow, force_get, has_content_type;
static const char *method_opt, *output_file;
static stralloc extra_headers, base, location;
static buffer file_out, *out = 0;
/* the body of a POST, read from standard input once when there are several urls */
static stralloc body;
static int stream_body;

static void
usage(const char* argv0) {
  buffer_putm_internal(buffer_1,
                       "Usage: ",
                       str_basename(argv0),
                       " [OPTIONS] URL...\n"
                       "\n"
                       "Requests every URL and writes the response bodies to standard output.\n"
                       "When standard input is a pipe or a file with data, it is sent as the\n"
                       "body of a POST in chunks.\n"
                       "\n"
                       "  -i, --include        write the status line and headers before the body\n"
                       "  -H, --header LINE    send an extra header, may be given more than once\n"
                       "  -X, --request NAME   use this method instead of GET or POST\n"
                       "  -G, --get            GET, do not send standard input\n"
                       "  -L, --location       follow redirects of a GET, up to 10\n"
                       "  -o, --output FILE    write to FILE instead of standard output\n"
                       "  -h, --help           show this help\n"
                       "\n"
                       "The exit status is 1 if a request failed or a status was 400 or more.\n",
                       NULL);
  buffer_flush(buffer_1);
}

static ssize_t
conn_read(fd_type fd, void* x, size_t n, buffer* b) {
  conn* c = b->cookie;
  return c->tls ? tls_read(fd, x, n) : read(fd, x, n);
}

static ssize_t
conn_write(fd_type fd, void* x, size_t n, buffer* b) {
  conn* c = b->cookie;
  return c->tls ? tls_write(fd, x, n) : write(fd, x, n);
}

/* copies n bytes, or everything when n is -1, returns -1 on a read error */
static int
copy(buffer* in, buffer* to, uint64 n) {
  char tmp[4096];
  ssize_t r;

  while(n) {
    size_t want = n < sizeof(tmp) ? (size_t)n : sizeof(tmp);

    if((r = buffer_get(in, tmp, want)) < 0)
      return -1;
    if(r == 0)
      break;

    buffer_put(to, tmp, (size_t)r);
    if(n != (uint64)-1)
      n -= (uint64)r;
  }
  buffer_flush(to);
  return 0;
}

/* a body can only be read from standard input once */
static int
body_wanted(void) {
  struct stat st;

  if(force_get)
    return 0;
  if(method_opt)
    return !(case_equals(method_opt, "GET") || case_equals(method_opt, "HEAD"));
  if(fstat(0, &st) < 0)
    return 0;
  /* a character device such as /dev/null or a terminal is not a body */
  return S_ISFIFO(st.st_mode) || S_ISSOCK(st.st_mode) || (S_ISREG(st.st_mode) && st.st_size > 0);
}

static void
send_body(conn* c) {
  buffer enc;
  char tmp[4096];
  ssize_t r;

  buffer_chunked_encode(&enc, &c->out);

  if(stream_body) {
    while((r = read(0, tmp, sizeof(tmp))) > 0)
      buffer_put(&enc, tmp, (size_t)r);
  } else {
    buffer_put(&enc, body.s, body.len);
  }
  buffer_close(&enc);
}

/* requests one url, returns the status or -1, location holds a redirect */
static int
fetch(const char* url, int with_body) {
  conn* c;
  uri_t u;
  stralloc line;
  const char* method;
  uint64 length = (uint64)-1;
  int status = -1, chunked = 0, tls, no_body;
  unsigned int code = 0;

  uri_init(&u);
  uri_scan(&u, url, str_len(url));
  location.len = 0;

  if(!u.host || !u.proto || !(str_equal(u.proto, "http") || str_equal(u.proto, "https"))) {
    errmsg_warn("not an http or https url: ", url, 0);
    return -1;
  }
  tls = str_equal(u.proto, "https");

  if(!(c = alloc_zero(sizeof(conn))))
    return -1;

  if((c->fd = address_connect(u.host, u.port)) < 0)
    goto done;

  c->tls = tls;

  if(tls) {
    tls_init(0, 0);

    if(!tls_client(c->fd) || tls_connect(c->fd) != 1) {
      errmsg_warn("TLS handshake with ", u.host, " failed: ", tls_strerror(c->fd), 0);
      goto closed;
    }
  }

  buffer_init(&c->in, conn_read, c->fd, c->inbuf, sizeof(c->inbuf));
  buffer_init(&c->out, conn_write, c->fd, c->outbuf, sizeof(c->outbuf));
  c->in.cookie = c->out.cookie = c;

  method = method_opt ? method_opt : with_body ? "POST" : "GET";

  buffer_putm_internal(&c->out, method, " ", u.location ? u.location : "/", " HTTP/1.1\r\nHost: ", u.host, NULL);
  if(u.port != (tls ? 443 : 80)) {
    buffer_putc(&c->out, ':');
    buffer_putulong(&c->out, u.port);
  }
  buffer_puts(&c->out, "\r\nUser-Agent: chttpcat\r\nAccept: */*\r\nAccept-Encoding: identity\r\nConnection: close\r\n");
  buffer_putsa(&c->out, &extra_headers);

  if(with_body) {
    if(!has_content_type)
      buffer_puts(&c->out, "Content-Type: application/octet-stream\r\n");
    buffer_puts(&c->out, "Transfer-Encoding: chunked\r\n");
  }
  buffer_puts(&c->out, "\r\n");

  if(with_body)
    send_body(c);
  else
    buffer_flush(&c->out);

  stralloc_init(&line);

  if(buffer_getline_sa(&c->in, &line) <= 0) {
    errmsg_warn("no response from ", u.host, 0);
    goto closed;
  }
  stralloc_trimr(&line, "\r\n", 2);
  stralloc_nul(&line);

  if(line.len < 12 || !scan_uint(line.s + str_chr(line.s, ' ') + 1, &code)) {
    errmsg_warn("bad status line from ", u.host, ": ", line.s, 0);
    goto closed;
  }
  status = (int)code;

  if(show_headers) {
    buffer_putsa(out, &line);
    buffer_puts(out, "\r\n");
  }

  for(;;) {
    stralloc_zero(&line);

    if(buffer_getline_sa(&c->in, &line) <= 0)
      break;

    stralloc_trimr(&line, "\r\n", 2);
    stralloc_nul(&line);

    if(show_headers) {
      buffer_putsa(out, &line);
      buffer_puts(out, "\r\n");
    }

    if(line.len == 0)
      break;

    if(case_starts(line.s, "content-length:"))
      scan_u64(line.s + 15 + scan_whitenskip(line.s + 15, line.len - 15), &length);
    else if(case_starts(line.s, "transfer-encoding:") &&
            case_starts(line.s + 18 + scan_whitenskip(line.s + 18, line.len - 18), "chunked"))
      chunked = 1;
    else if(case_starts(line.s, "location:")) {
      stralloc_copys(&location, line.s + 9 + scan_whitenskip(line.s + 9, line.len - 9));
      stralloc_nul(&location);
    }
  }

  /* the base for a redirect to a path */
  stralloc_copys(&base, u.proto);
  stralloc_cats(&base, "://");
  stralloc_cats(&base, u.host);
  if(u.port != (tls ? 443 : 80)) {
    stralloc_cats(&base, ":");
    stralloc_catulong(&base, u.port);
  }

  no_body = case_equals(method, "HEAD") || status == 204 || status == 304 || (status >= 100 && status < 200);

  if(!no_body) {
    if(chunked) {
      buffer dec;

      buffer_chunked(&dec, &c->in);
      if(copy(&dec, out, (uint64)-1) < 0) {
        errmsg_warnsys("reading the chunked body from ", u.host, 0);
        status = -1;
      }
      buffer_close(&dec);
    } else if(copy(&c->in, out, length) < 0) {
      errmsg_warnsys("reading the body from ", u.host, 0);
      status = -1;
    }
  }
  buffer_flush(out);

closed:
  if(tls)
    tls_close(c->fd);
  close(c->fd);
done:
  uri_free(&u);
  alloc_free(c);
  return status;
}

/* the url of a redirect, absolute or a path on the same host, 0 if neither */
static const char*
redirect_target(stralloc* next) {
  if(case_starts(location.s, "http://") || case_starts(location.s, "https://"))
    stralloc_copy(next, &location);
  else if(location.s[0] == '/') {
    stralloc_copy(next, &base);
    stralloc_cat(next, &location);
  } else
    return 0;

  stralloc_nul(next);
  return next->s;
}

int
main(int argc, char* argv[]) {
  static const struct unix_longopt opts[] = {{"include", 0, NULL, 'i'},
                                             {"header", 1, NULL, 'H'},
                                             {"request", 1, NULL, 'X'},
                                             {"get", 0, NULL, 'G'},
                                             {"location", 0, NULL, 'L'},
                                             {"output", 1, NULL, 'o'},
                                             {"help", 0, NULL, 'h'},
                                             {0, 0, 0, 0}};
  int c, index = 0, i, rc = 0, urls, with_body;

  errmsg_iam(argv[0]);

  while((c = unix_getopt_long(argc, argv, "iH:X:GLo:h", opts, &index)) != -1) {
    switch(c) {
      case 'i': show_headers = 1; break;
      case 'H':
        stralloc_cats(&extra_headers, unix_optarg);
        stralloc_cats(&extra_headers, "\r\n");
        if(case_starts(unix_optarg, "content-type:"))
          has_content_type = 1;
        break;
      case 'X': method_opt = unix_optarg; break;
      case 'G': force_get = 1; break;
      case 'L': follow = 1; break;
      case 'o': output_file = unix_optarg; break;
      case 'h': usage(argv[0]); return 0;
      default: return 2;
    }
  }

  urls = argc - unix_optind;

  if(urls < 1) {
    usage(argv[0]);
    return 2;
  }

  if(output_file && !str_equal(output_file, "-")) {
    if(buffer_truncfile(&file_out, output_file) < 0) {
      errmsg_warnsys("cannot write ", output_file, 0);
      return 1;
    }
    out = &file_out;
  } else {
    out = buffer_1;
  }

  with_body = body_wanted();

  if(with_body && urls > 1) {
    char tmp[4096];
    ssize_t r;

    while((r = read(0, tmp, sizeof(tmp))) > 0)
      stralloc_catb(&body, tmp, (size_t)r);
  } else {
    stream_body = with_body;
  }

  for(i = unix_optind; i < argc; i++) {
    stralloc next;
    const char* url = argv[i];
    int hops, code;

    stralloc_init(&next);

    for(hops = 0;; hops++) {
      code = fetch(url, with_body);

      if(!with_body && follow && hops < 10 &&
         (code == 301 || code == 302 || code == 303 || code == 307 || code == 308) && location.len &&
         (url = redirect_target(&next)))
        continue;

      break;
    }

    if(code < 0 || code >= 400)
      rc = 1;

    /* standard input is gone after the first url when it was streamed */
    if(stream_body)
      with_body = 0;
  }

  buffer_flush(out);
  return rc;
}
