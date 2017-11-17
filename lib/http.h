#ifndef HTTP_H
#define HTTP_H 1

#include "uint16.h"
#include "uint32.h"
#include "uint64.h"
#include "stralloc.h"
//#include "buffer.h"

#ifdef __cplusplus
extern "C" {
#endif


struct http_request_s;

typedef struct http_request_s {
  uint32 serial;
  enum { GET=0, POST } type;
  stralloc location;
  struct http_request_s* next;
} http_request;

struct http_response_s;

typedef struct http_response_s {
  enum { DEFAULT=0, CLOSED, ERROR } status;
  enum { START=0, HEADER=1, CHUNKS=2 } part;
  stralloc body;
  stralloc data;
  size_t ptr;
//  buffer rbuf;
} http_response;

typedef struct {
  int64 sock;
  uint16 port;
  stralloc host;
  char addr[4];
  http_request* request;
  http_response* response;
} http;

int http_get(http *h, const char *location);
void http_init(http *h, const char *host, uint16 port);
int http_sendreq(http *h);
void http_readable();
void http_writeable();


#ifdef __cplusplus
}
#endif


#endif /* defined HTTP_H */
