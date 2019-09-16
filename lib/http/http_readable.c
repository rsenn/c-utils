#define USE_WS2_32 1
#include "../socket_internal.h"
#include "../windoze.h"

#include "../buffer.h"
#include "../byte.h"
#include "../http.h"
#include "../io_internal.h"
#include "../scan.h"
#include "../stralloc.h"
#include "../str.h"
#include <ctype.h>
#include <errno.h>

#ifndef EAGAIN
#define EAGAIN 11
#endif

#define is_space(c) ((c) == ' ' || (c) == '\t' || (c) == '\r' || (c) == '\n')

extern ssize_t buffer_dummyread(int, void*, size_t, void*);

static int
boundary_predicate(stralloc* sa, void* arg) {
  stralloc* pred = arg;

  if(pred->len >= sa->len) {
    if(!byte_diff(&sa->s[sa->len - pred->len], pred->len, pred->s))
      return 1;
  }
  return 0;
}

int
http_readable(http* h, int freshen) {
  ssize_t ret = -1;
  int err;
  http_response* r;

  if(freshen)
    buffer_freshen(&h->q.in);

  if((r = h->response) == NULL) {
    return ret;
  }

  while(r->status == HTTP_RECV_HEADER) {
    if((ret = buffer_getline_sa(&h->q.in, &r->data)) <= 0)
      break;

    stralloc_trimr(&r->data, "\r\n", 2);
    stralloc_nul(&r->data);

    // putline("Header", r->data.s, -r->data.len, &h->q.in);

    if(r->data.len == 0) {
      r->ptr = 0;
      r->status = HTTP_RECV_DATA;

      if(h->q.in.p < h->q.in.n) {
        ret = 1;
      } else {
        ret = 0;
        errno = EWOULDBLOCK;
      }
      break;
    }

    if(stralloc_startb(&r->data, "Content-Type: multipart", 23)) {
      size_t p = str_find(r->data.s, "boundary=");

      if(r->data.s[p]) {
        stralloc_copys(&r->boundary, &r->data.s[p + str_len("boundary=")]);
      }
      r->transfer = HTTP_TRANSFER_BOUNDARY;
    } else if(stralloc_startb(&r->data, "Content-Length:", 15)) {
      scan_ulonglong(&r->data.s[16], &r->content_length);
      r->transfer = HTTP_TRANSFER_LENGTH;
    } else {
      r->transfer = HTTP_TRANSFER_CHUNKED;
    }

    stralloc_zero(&r->data);
  }

  if(r->status == HTTP_RECV_DATA) {
    ret = 1;

    /*
        if(r->content_length) {
          size_t a;

          if(r->content_length < (a = r->data.a - r->data.len))
            a = r->content_length;
          if(a > 1024)
            a = 1024;

          stralloc_readyplus(&r->data, 1024);

          if((ret = buffer_get(&h->q.in, &r->data.s[r->data.len], r->data.a - r->data.len)) <= 0)
            break;

          putline("data", &r->data.s[r->data.len], 1, &h->q.in);

          r->data.len += ret;
          r->content_length -= ret;
          continue;
        }

        r->status = HTTP_STATUS_FINISH;*/
  }

  if(ret == -1) {
    err = errno;
    errno = 0;
  } else {
    err = 0;
  }

  if(ret == 0) {
    r->status = HTTP_STATUS_FINISH;
  }

  if(err && err != EWOULDBLOCK)
    r->status = HTTP_STATUS_ERROR;

  return ret;
}

