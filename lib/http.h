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

struct http_s;
struct http_request_s;

typedef enum { GET = 0, POST } http_req_t;

typedef struct http_request_s {
  uint32 serial;
  http_req_t type;
  stralloc location;
  stralloc headers;
  struct http_request_s* next;
} http_request;

typedef struct http_return_s {
  int ret;
  int err;
} http_return_value;

struct http_response_s;
typedef enum { HTTP_TRANSFER_UNDEF = 0, HTTP_TRANSFER_CHUNKED, HTTP_TRANSFER_LENGTH, HTTP_TRANSFER_BOUNDARY } http_transfer_type;

typedef enum { HTTP_RECV_HEADER = 0, HTTP_RECV_DATA, HTTP_STATUS_CLOSED, HTTP_STATUS_ERROR, HTTP_STATUS_BUSY, HTTP_STATUS_FINISH } http_status;

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
  int (*header)(struct http_s*, const char*, size_t);
  //  buffer rbuf;
} http_response;

typedef struct http_s {
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

void http_close(http*);
size_t http_errstr(int, char*, size_t);
int http_get(http*, const char*);
void http_init(http*, const char*, uint16);
ssize_t http_on_readable(http*, void (*)(fd_t));
ssize_t http_on_writeable(http*, void (*)(fd_t));
ssize_t http_readable(http*, int);
ssize_t http_read(fd_t, char*, size_t, void*);
ssize_t http_read_header(http*, stralloc*, http_response*);
size_t http_read_internal(http*, char*, size_t);
int http_sendreq(http*);
int http_socket(http*, int);
ssize_t http_socket_read(fd_t, void*, size_t, void*);
ssize_t http_socket_write(fd_t, void*, size_t, void*);
#ifdef HAVE_OPENSSL
ssize_t http_ssl2errno(http*, ssize_t);
ssize_t http_ssl2want(http*, ssize_t, void (*)(fd_t), void (*)(fd_t));

ssize_t http_ssl_connect(http*);
void* http_ssl_ctx(void);
const char* http_ssl_errflag(int);
size_t http_ssl_errstr(int, char*, size_t);
int http_ssl_errno(int);
ssize_t http_ssl_io_errhandle(http*, int);
ssize_t http_ssl_io_want(http*, int);
ssize_t http_ssl_read(fd_t, void*, size_t, void*);
int http_ssl_socket(http*);
ssize_t http_ssl_write(fd_t, const void*, size_t, void*);
#endif

#ifdef __cplusplus
}
#endif

#endif /* defined HTTP_H */
