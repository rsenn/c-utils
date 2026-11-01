#define USE_WS2_32 1
#include "../socket_internal.h"
#include "../stralloc.h"
#include "../buffer.h"
#include "../http.h"
#include "../byte.h"
#include "../io.h"
#include "../fmt.h"
#include "../str.h"
#include "../case.h"

/* tells if one of the header lines is called name */
static int
header_has(const stralloc* headers, const char* name) {
  size_t i = 0, len = str_len(name);

  while(i < headers->len) {
    if(headers->len - i > len && headers->s[i + len] == ':' && !case_diffb(&headers->s[i], len, name))
      return 1;

    i += byte_chr(&headers->s[i], headers->len - i, '\n');
    if(i < headers->len)
      i++;
  }

  return 0;
}

int
http_sendreq(http* h) {
  int ret;
  size_t len;
  buffer* out = &h->q.out;

  if(h->request == NULL)
    return 0;

  if(!h->request->queued) {
    http_request* req = h->request;
    int v11 = h->version == 11 || req->chunked || req->type != GET;
    int hasbody = req->chunked || req->body.len || req->body_done || req->type == POST || req->type == PUT || req->type == PATCH;
    size_t n;
    char num[FMT_ULONG];

    buffer_puts(out, http_method(req->type));
    buffer_puts(out, " ");
    buffer_putsa(out, &req->location);
    buffer_puts(out, v11 ? " HTTP/1.1\r\n" : " HTTP/1.0\r\n");
    buffer_puts(out, "Host: ");
    buffer_putsa(out, &h->host);

    if(h->port && h->port != (h->tls ? 443 : 80)) {
      buffer_puts(out, ":");
      buffer_put(out, num, fmt_ulong(num, h->port));
    }
    buffer_puts(out, "\r\n");

    if(!h->keepalive)
      buffer_puts(out, "Connection: close\r\n");

    if(!header_has(&req->headers, "Accept"))
      buffer_puts(out, "Accept: */*\r\n");

    if(req->headers.len)
      buffer_putsa(out, &req->headers);

    if(req->chunked) {
      buffer_puts(out, "Transfer-Encoding: chunked\r\n");
    } else if(hasbody) {
      n = req->body.len;
      buffer_puts(out, "Content-Length: ");
      buffer_put(out, num, fmt_ulong(num, n));
      buffer_puts(out, "\r\n");
    }

    buffer_puts(out, "\r\n");
    req->queued = 1;
  }

#ifdef DEBUG_OUTPUT
  buffer_puts(buffer_2, "Request:\n");

  {
    const char* x = out->x;
    ssize_t i, n = out->p;

    while(n > 0) {
      i = byte_chr(x, n, '\r');

      buffer_put(buffer_2, x, i);
      buffer_putnlflush(buffer_2);

      x += i + 1;
      n -= i + 1;

      if(*x == '\n') {
        x++;
        n--;
      }
    }
  }

  buffer_flush(buffer_2);
#endif

  len = out->p;

#ifdef DEBUG_HTTP
  buffer_putspad(buffer_2, "\x1b[38;5;208mhttp_sendreq\x1b[0m ", 30);
  buffer_puts(buffer_2, "location=");
  buffer_putsa(buffer_2, &h->request->location);

  if(h->response->code != -1) {
    buffer_puts(buffer_2, " code=");
    buffer_putlong(buffer_2, h->response->code);
  }

  if(len > 0) {
    buffer_puts(buffer_2, " len=");
    buffer_putlong(buffer_2, len);
  }

  if(out->n > out->p) {
    buffer_puts(buffer_2, " code=");
    buffer_putfmt(buffer_2, out->x, out->p, &fmt_escapecharnonprintable);
  }

  buffer_puts(buffer_2, " status=");
  buffer_puts(buffer_2,
              ((const char* const[]){
                  "0", "HTTP_RECV_HEADER", "HTTP_RECV_DATA", "HTTP_STATUS_CLOSED", "HTTP_STATUS_ERROR", "HTTP_STATUS_BUSY", "HTTP_STATUS_FINISH", 0})[h->response->status]);
  buffer_putnlflush(buffer_2);
  buffer_flush(buffer_2);
#endif

  ret = buffer_flush(out);

#ifdef DEBUG_HTTP
  buffer_putspad(buffer_2, "\x1b[38;5;208mhttp_sendreq\x1b[0m ", 30);
  buffer_puts(buffer_2, "ret=");
  buffer_putlong(buffer_2, ret);

  if(ret < 0) {
    buffer_puts(buffer_2, " err=");
    buffer_putstr(buffer_2, http_strerror(h, ret));
  }

  buffer_putnlflush(buffer_2);
#endif

  if(ret != -1) {
    h->sent = 1;

    io_onlywantread(h->sock);

    /* the body, what does not fit now is written when the socket is writable */
    if(http_body_flush(h) < 0)
      ret = -1;
  }

  return ret;
}
