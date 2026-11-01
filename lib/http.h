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

typedef enum {
  GET = 0,
  POST,
  PUT,
  DELETE,
  PATCH,
  HEAD,
} http_req_t;

typedef struct http_request_s {
  uint32 serial;
  http_req_t type;
  stralloc location;
  /* extra header lines, each ends in CRLF */
  stralloc headers;
  /* what follows the headers, a chunked body is stored encoded */
  stralloc body;
  size_t body_pos;
  unsigned chunked : 1;
  unsigned body_done : 1;
  unsigned queued : 1;
  struct http_request_s* next;
} http_request;

typedef struct http_return_s {
  int ret;
  int err;
} http_return_value;

struct http_response_s;
typedef enum {
  HTTP_TRANSFER_UNDEF = 0,
  HTTP_TRANSFER_CHUNKED,
  HTTP_TRANSFER_LENGTH,
  HTTP_TRANSFER_BOUNDARY,
} http_transfer_type;

typedef enum {
  HTTP_RECV_HEADER = 1,
  HTTP_RECV_DATA,
  HTTP_STATUS_CLOSED,
  HTTP_STATUS_ERROR,
  HTTP_STATUS_BUSY,
  HTTP_STATUS_FINISH,
} http_status;

typedef struct http_response_s {
  http_transfer_type transfer;
  http_status status;
  int code;
  uint64 headers_len, data_pos;
  uint64 content_length;
  uint64 chunk_length;
  /* where http_recv is in a chunked body, 0 is the size line */
  int chunk_state;
  stralloc data, boundary;
  int err;
  int (*header)(struct http_s*, const char*, size_t);
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
  unsigned nonblocking : 1;
  unsigned keepalive : 1;
  unsigned connected : 1;
  unsigned tls : 1;
  unsigned sent : 1;
  /* leave the body in the input buffer for http_recv instead of collecting it in the response */
  unsigned stream : 1;
  uint16 version;
} http;

ssize_t http_canread(http* h, void (*wantread)(fd_type), void (*wantwrite)(fd_type));
ssize_t http_canwrite(http* h, void (*wantread)(fd_type), void (*wantwrite)(fd_type));
void http_close(http* h);
void http_free(http* h);

void http_dump(http*);
/* b reads the chunked transfer encoding of other and decodes it */
int buffer_chunked(buffer* b, buffer* other);

/* b writes what is put into it to other as chunks, closing b ends them */
int buffer_chunked_encode(buffer* b, buffer* other);

char* http_get_header(char* data, size_t n, const char* name, size_t* result_len);
int http_get(http* h, const char* location);

/* starts a request with any method, GET and POST are the same with the method fixed */
int http_open(http* h, http_req_t type, const char* location);
int http_post(http* h, const char* location);

/* adds a header line to the request that was started last */
int http_header(http* h, const char* name, const char* value);

/* sets a body of known length, sent with a Content-Length header */
int http_body(http* h, const char* x, size_t n);

/* sends the body as chunks. http_chunked declares it before the request is
 * sent, http_chunk queues data (it declares it too), http_chunk_end queues
 * the last chunk. They can be called while the request is being sent. */
int http_chunked(http* h);
int http_chunk(http* h, const char* x, size_t n);
int http_chunk_end(http* h);

/* Decodes the body of the response from what http_canread received, whatever
 * the transfer encoding is, without reading from the socket. Returns the
 * number of bytes stored in buf, 0 if there is nothing now (the response is
 * complete when its status is HTTP_STATUS_FINISH) and -1 on an error. Set
 * h->stream before the request is sent to use it. */
ssize_t http_recv(http* h, char* buf, size_t len);

/* writes the queued body, 1 when all is out, 0 when the socket is full, -1 on error */
ssize_t http_body_flush(http* h);

/* a method as upper case text */
const char* http_method(http_req_t type);
void http_init(http* h, const char* host, uint16 port);
ssize_t http_read(fd_type, char*, size_t, buffer*);
ssize_t http_read_header(buffer*, stralloc*, http_response*);
ssize_t http_read_internal(fd_type, char*, size_t, buffer*);

void http_response_dump(http_response* r);
void http_response_free(http_response* r);
http_response* http_response_new(http*);
ssize_t http_response_read(buffer*, http_response*);

void http_request_dump(http_request*);
http_request* http_request_new(http*);
int http_sendreq(http* h);
size_t http_skip_header(const char* x, size_t len);

int http_socket(http*, int);
ssize_t http_socket_read(fd_type, void*, size_t, buffer*);
ssize_t http_socket_write(fd_type, void*, size_t, buffer*);

const char* http_strerror(http*, int);

#ifdef __cplusplus
}
#endif

#endif /* defined HTTP_H */
/** @} */
