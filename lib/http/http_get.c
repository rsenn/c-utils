#include "../buffer.h"
#include "../byte.h"
#include "../http.h"
#include "../io.h"
#include "../io_internal.h"
#include "../ip4.h"
#include "../socket.h"
#include "../str.h"

#include <errno.h>
#include <stdlib.h>

#include <netdb.h>

int
http_get(http* h, const char* location) {
  int ret;
  struct hostent* he;
  ipv4addr a;
  uint32 serial = 0;

  stralloc_0(&h->host);
  h->host.len = str_len(h->host.s);
  he = gethostbyname(h->host.s);
  if(he == NULL) return 0;
  a = *((ipv4addr**)(he->h_addr_list))[0];
  if(a.iaddr == 0) return 0;
  byte_copy(h->addr, sizeof(h->addr), &a.iaddr);
  buffer_putsa(buffer_1, &h->host);
  buffer_puts(buffer_1, " (");
  buffer_putulong(buffer_1, a.iaddr & 0xff);
  buffer_puts(buffer_1, ".");
  buffer_putulong(buffer_1, (a.iaddr >> 8) & 0xff);
  buffer_puts(buffer_1, ".");
  buffer_putulong(buffer_1, (a.iaddr >> 16) & 0xff);
  buffer_puts(buffer_1, ".");
  buffer_putulong(buffer_1, (a.iaddr >> 24) & 0xff);
  buffer_puts(buffer_1, ")");
  buffer_putnlflush(buffer_1);
  h->sock = socket_tcp4();
  io_nonblock(h->sock);

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
    stralloc_init(&((*r)->body));
    stralloc_init(&((*r)->data));
    stralloc_init(&((*r)->boundary));
  }
  ret = socket_connect4(h->sock, h->addr, h->port);

  if(ret == -1) {

    if(errno == EINPROGRESS) {
      ret = 0;
      errno = 0;
    }
  }
  return ret == 0;
}
