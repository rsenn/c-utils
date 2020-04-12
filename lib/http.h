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
  

typedef enum { GET = 0, POST } http_req_t;

typedef struct http_request_s {
  uint32 serial;
  http_req_t type;
  stralloc location;
  stralloc headers;
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
  int err;
  int nonblocking : 1;
  int keepalive : 1;
  int connected : 1;
  int tls : 1;
  int sent : 1;
} http;

void http_close(http* h);
int http_get(http* h, const char* location);
void http_init(http* h, const char* host, uint16 port);
ssize_t http_readable(http* h, int freshen);
ssize_t http_read(fd_t fd, char* buf, size_t len, void* ptr);
ssize_t http_read_header(http* h, http_response* r);
size_t http_read_internal(http* h, char* buf, size_t len);
int http_sendreq(http* h);
int http_socket(http* h, int nonblock);
ssize_t http_socket_read(fd_t fd, void* buf, size_t len, void* b);
ssize_t http_socket_write(fd_t fd, void* buf, size_t len, void* b);
ssize_t http_ssl_connect(http* h);
void* http_ssl_ctx(void);
const char* http_ssl_errflag(int i);
int http_ssl_errno(int err);
int http_ssl_error(http* h, ssize_t ret);
ssize_t http_ssl_io_again(http* h, ssize_t ret);
ssize_t http_ssl_io_errhandle(http* h, int err);
ssize_t http_ssl_read(fd_t fd, void* buf, size_t len, void* b);
int http_ssl_socket(http* h);
ssize_t http_ssl_write(fd_t fd, const void* buf, size_t n, void* b);
ssize_t http_writeable(http* h);

#ifdef __cplusplus
}
#endif

#endif /* defined HTTP_H */
