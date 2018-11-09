#define USE_WS2_32 1
#include "../socket_internal.h"
#include "../buffer.h"
#include "../byte.h"
#include "../dns.h"
#include "../errmsg.h"
#include "../http.h"
#include "../io.h"
#include "../io_internal.h"
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

  stralloc_nul(&h->host);

  stralloc_init(&dns);
  if(dns_ip4(&dns, &h->host) == -1) {
    errmsg_warnsys("ERROR: resolving ", h->host.s, ": ", NULL);
    return 0;
  }

  a = (void*)dns.s;

  byte_copy(&h->addr, sizeof(ipv4addr), &a->iaddr);

  buffer_putsa(buffer_2, &h->host);
  buffer_puts(buffer_2, " (");
  buffer_put(buffer_2, ip, fmt_ip4(ip, (const char*)a->addr));
  buffer_puts(buffer_2, ")");
  buffer_putnlflush(buffer_2);

  http_socket(h);

  if(h->request) {
    serial = h->request->serial + 1;
    free(h->request);
    h->request = NULL;
  }
  {
    http_request** r = &h->request;
    (*r) = malloc(sizeof(http_request));
    byte_zero((*r), sizeof(http_request));
    (*r)->serial = serial;
    (*r)->type = GET;
    stralloc_init(&((*r)->location));
    stralloc_copys(&((*r)->location), location);
  }
  {
    http_response** r = &h->response;
    (*r) = malloc(sizeof(http_response));
    byte_zero((*r), sizeof(http_response));
    stralloc_init(&((*r)->data));
    stralloc_init(&((*r)->boundary));
  }
  ret = socket_connect4(h->sock, (const char*)h->addr.addr, h->port);

  if(ret == -1) {

    if(errno == EINPROGRESS) {
      ret = 0;
      errno = 0;
    }
  }
  return ret == 0;
}
