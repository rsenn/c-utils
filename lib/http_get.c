#include "http.h"
#include "socket.h"
#include "str.h"
#include "byte.h"
#include "io.h"
#include <stdlib.h>
#include <errno.h>
#include "socket.h"

#ifndef _WIN32
#include <netdb.h>
#include <netinet/in.h>
#endif



#include "buffer.h"

int
http_get(http* h, const char* location) {
   struct hostent *he;
  struct in_addr a;

  stralloc_0(&h->host);
  h->host.len = str_len(h->host.s);
  he = gethostbyname(h->host.s);

  if(he == NULL)
    return 0;
  a = *((struct in_addr**)(he->h_addr_list))[0];

  if(a.s_addr == 0)
    return 0;

  byte_copy(h->addr, sizeof(h->addr), &a.s_addr);
//  uint32_pack_big(h->addr, a.s_addr);


  buffer_putsa(buffer_2, &h->host);
  buffer_puts(buffer_2, " (");
  buffer_putulong(buffer_2, a.s_addr & 0xff); buffer_puts(buffer_2, ".");
  buffer_putulong(buffer_2, (a.s_addr >> 8) & 0xff); buffer_puts(buffer_2, ".");
  buffer_putulong(buffer_2, (a.s_addr >> 16) & 0xff); buffer_puts(buffer_2, ".");
  buffer_putulong(buffer_2, (a.s_addr >> 24) & 0xff);
//  buffer_puts(buffer_2, ", ");
//  buffer_putsa(buffer_2, &h->addr);
  buffer_puts(buffer_2, ")");
  buffer_putnlflush(buffer_2);


//  h->addr.len = str_len(h->addr.s);

  h->sock = socket_tcp4();
  io_nonblock(h->sock);

  uint32 serial = 0;
  if(h->request) {
    serial = h->request->serial + 1;
    free(h->request);
    h->request = NULL;
  }
//  for(r = &h->request; *r && (*r)->next; r = &(*r)->next)  {}

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
  }


  int ret = socket_connect4(h->sock, h->addr, h->port);

  if(ret == -1) {

    if(errno == EINPROGRESS) {
      ret = 0;
      errno = 0;
    }
  }
  return ret == 0;
}
