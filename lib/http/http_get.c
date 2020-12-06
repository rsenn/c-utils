#define USE_WS2_32 1
#include "../alloc.h"
#include "../socket_internal.h"
#include "../socket.h"
#include "../scan.h"
#include "../buffer.h"
#include "../byte.h"
#include "../dns.h"
#include "../errmsg.h"
#include "../http.h"
#include "../io.h"
#include "../ip4.h"
#include "../str.h"

#include <errno.h>
#include <stdlib.h>
#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))

int
http_get(http* h, const char* location) {
  int ret;
  struct hostent* he;
  ipv4addr* a;
  char ip[FMT_IP4];
  stralloc dns;
  uint32 serial = 0;
  size_t len = byte_chrs(location, str_len(location), "\r\n\0", 3);
#ifdef DEBUG_HTTP
  buffer_puts(buffer_2, "http_get ");

  buffer_put(buffer_2, location, len);
  buffer_puts(buffer_2, " host=");
  buffer_putsa(buffer_2, &h->host);
  buffer_putnlflush(buffer_2);
#endif
  if(location[0] != '/') {
    size_t pos;
    pos = str_findb(location, "://", 3);
    if(location[pos]) {
      h->tls = len >= 5 && !byte_diff(location, 5, "https");
      location += pos + 3;
    }
    pos = str_chrs(location, "/:", 2);
    stralloc_copyb(&h->host, location, pos);
    if(location[pos] == ':') {
      pos += 1;
      pos += scan_ushort(&location[pos], &h->port);
    } else {
      h->port = h->tls ? 443 : 80;
    }
    location += pos;
  }

  if(byte_equal(&h->addr, sizeof(ipv4addr), &IPV4ADDR_ANY)) {
    stralloc_nul(&h->host);
    stralloc_init(&dns);
    if(dns_ip4(&dns, &h->host) == -1) {
      errmsg_warnsys("ERROR: resolving ", h->host.s, ": ", NULL);
      return 0;
    }
    a = (ipv4addr*)dns.s;
    byte_copy(&h->addr, sizeof(ipv4addr), &a->iaddr);
#ifdef DEBUG_HTTP
    buffer_putspad(buffer_2, "http_get resolved ", 30);
    buffer_putsa(buffer_2, &h->host);
    buffer_puts(buffer_2, " to (");
    buffer_put(buffer_2, ip, fmt_ip4(ip, (const char*)&h->addr));
    buffer_puts(buffer_2, ")");
    buffer_putnlflush(buffer_2);
#endif
  }
  http_socket(h, h->nonblocking);
  /*  if(h->request) {
      serial = h->request->serial + 1;
      free(h->request);
      h->request = NULL;
    }
  */
  {
    http_request* req = (http_request*)alloc_zero(sizeof(http_request));
    req->serial = serial;
    req->type = GET;

    stralloc_init(&req->location);
    stralloc_catb(&req->location, location, min(len, str_len(location)));
    stralloc_nul(&req->location);
    stralloc_init(&req->headers);

    req->next = h->request;
    h->request = req;
  }

  {
    http_response* res = http_response_new();
    res->next = h->response;
    res->status = -1;
    res->code = -1;

    h->response = res;
  }

  h->connected = 0;
  h->sent = 0;

  ret = socket_connect4(h->sock, (const char*)&h->addr, h->port);
  if(ret == -1) {
    if(errno == EINPROGRESS) {
      ret = 0;
      errno = 0;
    }
  }
  io_onlywantwrite(h->sock);
#ifdef DEBUG_HTTP
  buffer_putspad(buffer_2, "http_get", 30);
  buffer_puts(buffer_2, "ret=");
  buffer_putlong(buffer_2, ret);
  buffer_putnlflush(buffer_2);
#endif
  return ret == 0;
}
