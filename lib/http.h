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
  HTTP_RECV_HEADER = 0,
  HTTP_RECV_DATA,
  HTTP_STATUS_CLOSED,
  HTTP_STATUS_ERROR,
  HTTP_STATUS_BUSY,
  HTTP_STATUS_FINISH
} http_status;

typedef struct http_response_s {
  http_transfer_type transfer;
  http_status status;
  stralloc data;
  size_t ptr;
  size_t chnk;
  size_t line;
  union {
    uint64 content_length;
    uint64 chunk_length;
  };
  stralloc boundary;
  int err;
  //  buffer rbuf;
} http_response;

typedef struct {
  fd_t sock;
  struct {
    buffer out;
    buffer in;
  } q;
  buffer data;
  stralloc host;
  ipv4addr addr;
  ipv4port port;
  http_request* request;
  http_response* response;
  void* ssl;
  int nonblocking;
} http;

int http_get(http*, const char* location);
void http_init(http*, const char* host, uint16 port);
int http_readable(http*, int freshen);
ssize_t http_read_header(http*, http_response* r);
size_t http_read_internal(http*, char* buf, size_t len);
ssize_t http_read(http*, char* buf, size_t len);
int http_sendreq(http*);
ssize_t http_socket_read(fd_t, void* buf, size_t len, buffer* b);
int http_socket(http*, int nonblock);
void http_writeable(void);

#ifdef __cplusplus
}
#endif

#endif /* defined HTTP_H */
