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

int
http_get(http* h, const char* location) {
  int ret;
  struct hostent* he;
  ipv4addr* a;
  char ip[FMT_IP4];
  stralloc dns;
  uint32 serial = 0;
  size_t len = str_len(location);
  h->tls = len >= 5 && !byte_diff(location, 5, "https");
#ifdef DEBUG_HTTP
  buffer_putm_internal(buffer_2, "http_get ", location, "\n", NULL);
  buffer_flush(buffer_2);
#endif
  if(location[0] != '/') {
    size_t len;
    len = str_findb(location, "://", 3);
    if(location[len])
      location += len + 3;
    len = str_chrs(location, "/:", 2);
    stralloc_copyb(&h->host, location, len);
    if(location[len] == ':') {
      len += 1;
      len += scan_ushort(&location[len], &h->port);
    } else {
      h->port = h->tls ? 443 : 80;
    }
    location += len;
  }
  stralloc_nul(&h->host);
  stralloc_init(&dns);
  if(dns_ip4(&dns, &h->host) == -1) {
    errmsg_warnsys("ERROR: resolving ", h->host.s, ": ", NULL);
    return 0;
  }
  a = (ipv4addr*)dns.s;
  byte_copy(&h->addr, sizeof(ipv4addr), &a->iaddr);
#ifdef DEBUG_HTTP
  buffer_putspad(buffer_2, "http_get resolved ", 18);
  buffer_putsa(buffer_2, &h->host);
  buffer_puts(buffer_2, " to (");
  buffer_put(buffer_2, ip, fmt_ip4(ip, (const char*)a->addr));
  buffer_puts(buffer_2, ")");
  buffer_putnlflush(buffer_2);
#endif
  http_socket(h, h->nonblocking);
  if(h->request) {
    serial = h->request->serial + 1;
    free(h->request);
    h->request = NULL;
  }
  {
    http_request* req = h->request = (http_request*)alloc_zero(sizeof(http_request));
    req->serial = serial;
    req->type = GET;
    stralloc_init(&(req->location));
    stralloc_copys(&(req->location), location);
  }
  h->response = http_response_new();

  ret = socket_connect4(h->sock, (const char*)h->addr.addr, h->port);
  h->connected = 0;
  if(ret == -1) {
    if(errno == EINPROGRESS) {
      ret = 0;
      errno = 0;
    }
  }
  io_wantwrite(h->sock);
  return ret == 0;
}
