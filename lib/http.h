#ifndef HTTP_H
#define HTTP_H 1

#include "uint16.h"
#include "uint32.h"
#include "uint64.h"
#include "stralloc.h"
#include "buffer.h"
#include "ip4.h"

#ifdef __cplusplus
extern "C" {
#endif

struct http_request_s;

typedef struct http_request_s {
  uint32 serial;
  enum { GET = 0, POST } type;
  stralloc location;
  struct http_request_s* next;
} http_request;

struct http_response_s;
typedef enum {
  HTTP_TRANSFER_UNDEF = 0,
  HTTP_TRANSFER_CHUNKED,
  HTTP_TRANSFER_LENGTH,
  HTTP_TRANSFER_BOUNDARY
} http_transfer_type;

typedef enum {
  HTTP_RECV_HEADER,
  HTTP_RECV_DATA,
  HTTP_STATUS_CLOSED,
  HTTP_STATUS_ERROR,
  HTTP_STATUS_BUSY,
  HTTP_STATUS_FINISH
} http_status;


typedef struct http_response_s {

  http_transfer_type transfer;
  http_status status;
  stralloc body;
  stralloc data;
  size_t ptr;
  size_t chnk;
  size_t line;
  union {
    size_t content_length;
    size_t chunk_length;
  };
  stralloc boundary;
//  buffer rbuf;
} http_response;

typedef struct {
  fd_t sock;
  struct {
    buffer out;
    buffer in;
  } q;
  stralloc host;
  ipv4addr addr;
  ipv4port port;
  http_request* request;
  http_response* response;
} http;

int http_get(http *h, const char *location);
void http_init(http *h, const char *host, uint16 port);
int http_sendreq(http *h);
void http_readable();
void http_writeable();

ssize_t http_recv(fd_t fd, void* buf, size_t len, void* b);
ssize_t http_send(fd_t fd, void* buf, size_t len, void* b);
int     http_socket(http* h);

#ifdef __cplusplus
}
#endif

#endif /* defined HTTP_H */

