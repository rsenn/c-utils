/**
 * @defgroup   http
 * @brief      HTTP module.
 * @{
 */
#ifndef HTTP_H
#define HTTP_H 1

#include "uint16.h"
#include "uint32.h"
#include "uint64.h"
#include "stralloc.h"
#include "buffer.h"
#include "ip4.h"
#include "tls.h"

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
  int code;
  stralloc data;
  uint64 ptr, received;
  uint64 content_length;
  uint64 chunk_length;
  //};
  stralloc boundary;
  int err;
  int (*header)(struct http_s*, const char*, size_t);
  //  buffer rbuf;
  struct http_response_s* next;
} http_response;

typedef struct http_s {
  fd_type sock;
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
  tls_t* ssl;
  int err;
  int nonblocking : 1;
  int keepalive : 1;
  int connected : 1;
  int tls : 1;
  int sent : 1;
  uint16 version;
} http;

ssize_t http_canread(http* h, void (*wantread)(fd_type), void (*wantwrite)(fd_type));
ssize_t http_canwrite(http* h, void (*wantread)(fd_type), void (*wantwrite)(fd_type));
void http_close(http* h);
const char* http_get_header(http* h, const char* name);
int http_get(http* h, const char* location);
void http_init(http* h, const char* host, uint16 port);
ssize_t http_read(fd_type fd, char* buf, size_t len, void* ptr);
ssize_t http_read_header(http* h, stralloc* sa, http_response* r);
ssize_t http_read_internal(fd_type fd, char* buf, size_t received, buffer* b);
void http_response_dump(http_response* r);
void http_response_free(http_response* r);
http_response* http_response_new(void);
int http_sendreq(http* h);
size_t http_skip_header(const char* x, size_t len);
int http_socket(http* h, int nonblock);
ssize_t http_socket_read(fd_type fd, void* buf, size_t len, void* b);
ssize_t http_socket_write(fd_type fd, void* buf, size_t len, void* b);
const char* http_strerror(http* h, int ret);

#ifdef __cplusplus
}
#endif

#endif /* defined HTTP_H */
/** @} */
