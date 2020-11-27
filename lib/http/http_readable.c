#include "../windoze.h"
#include "../errmsg.h"
#include "../buffer.h"
#include "../byte.h"
#include "../http.h"
#include "../io.h"
#include "../scan.h"
#include "../stralloc.h"
#include "../str.h"
#include "../socket.h"

#include <ctype.h>
#include <errno.h>

#ifndef EAGAIN
#define EAGAIN 11
#endif

#ifdef HAVE_OPENSSL
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <assert.h>
#endif

#define is_space(c) ((c) == ' ' || (c) == '\t' || (c) == '\r' || (c) == '\n')

extern ssize_t buffer_dummyread(int, void*, size_t, void*);

static int
boundary_predicate(stralloc* sa, void* arg) {
  stralloc* pred = (stralloc*)arg;
  if(pred->len >= sa->len) {
    if(!byte_diff(&sa->s[sa->len - pred->len], pred->len, pred->s))
      return 1;
  }
  return 0;
}

ssize_t
http_readable(http* h, int freshen) {
  ssize_t ret = 0;
  int err;
  http_response* r;
#if DEBUG_HTTP
  buffer_putspad(buffer_2, "http_readable ", 18);
  buffer_puts(buffer_2, "sock=");
  buffer_putlong(buffer_2, h->sock);
  buffer_putnlflush(buffer_2);
#endif
#ifdef HAVE_OPENSSL
  if(h->ssl) {
    if(!h->connected) {
      if((ret = https_connect(h)) == 1) {
        errno = EWOULDBLOCK;
        io_dontwantread(h->sock);
        io_wantwrite(h->sock);
        return -1;
      }
    }
  }
#endif
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
