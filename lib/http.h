#ifndef HTTP_H
#define HTTP_H 1

#include "uint16.h"
#include "uint32.h"
#include "uint64.h"
#include "stralloc.h"

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

typedef struct {
  int64 sock;
  stralloc host;
  uint8 addr[4];
  uint16 port;
  http_request* request;
} http;

/* lib/http_get.c */
int http_get(http *h, const char *location);
/* lib/http_init.c */
void http_init(http *h, const char *host, uint16 port);

#ifdef __cplusplus
}
#endif


#endif /* defined HTTP_H */
