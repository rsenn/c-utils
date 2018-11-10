#define USE_WS2_32 1
#include "../socket_internal.h"
#include "../windoze.h"

#include "../buffer.h"
#include "../byte.h"
#include "../http.h"
#include "../io_internal.h"
#include "../scan.h"
#include "../str.h"
#include <ctype.h>
#include <errno.h>

#ifndef EAGAIN
#define EAGAIN 11
#endif

#define is_space(c) ((c) == ' ' || (c) == '\t' || (c) == '\r' || (c) == '\n')

ssize_t
buffer_dummyread() {
  return 0;
}

static void
putline(const char* what, const char* b, ssize_t l, buffer* buf) {
  buffer_puts(buffer_2, what);
  buffer_puts(buffer_2, "[");
  buffer_putulong(buffer_2, l <= 0 ? -l : l);
  buffer_puts(buffer_2, "]");
  buffer_puts(buffer_2, ": ");
  if(l <= 0)
    buffer_puts(buffer_2, b);
  else {
    while(l-- > 0) buffer_put(buffer_2, b++, 1);
  }
  buffer_puts(buffer_2, " (bytes in recvb: ");
  buffer_putulong(buffer_2, buf->n - buf->p);
  buffer_puts(buffer_2, ")");
  buffer_putnlflush(buffer_2);
}

static int
boundary_predicate(stralloc* sa, void* arg) {
  stralloc* pred = arg;

  if(pred->len >= sa->len) {
    if(!byte_diff(&sa->s[sa->len - pred->len], pred->len, pred->s))
      return 1;
  }
  return 0;
}

void
http_readable(http* h) {
  ssize_t ret;
  int err;

  buffer_feed(&h->q.in);

  if(h->response) {
    http_response* r = h->response;

    if(r->status == HTTP_RECV_HEADER) {
      while((ret = buffer_getline_sa(&h->q.in, &r->data)) > 0) {
        stralloc_trim(&r->data, "\r\n", 2);
        stralloc_nul(&r->data);

        putline("Header", r->data.s, -r->data.len, &h->q.in);

        if(r->data.len == 0) {
          r->status = HTTP_RECV_DATA;
          break;
        }

        if(stralloc_startb(&r->data, "Content-Type: multipart", 23)) {
          size_t p = str_find(r->data.s, "boundary=");

          if(r->data.s[p]) {
            stralloc_copys(&r->boundary, &r->data.s[p + str_len("boundary=")]);
          }
          r->transfer = HTTP_TRANSFER_BOUNDARY;
        } else if(stralloc_startb(&r->data, "Content-Length:", 15)) {
          scan_ulong(&r->data.s[16], &r->content_length);
          r->transfer = HTTP_TRANSFER_LENGTH;
        } else {
          r->transfer = HTTP_TRANSFER_CHUNKED;
        }

        stralloc_zero(&r->data);
      }
    }

    while(r->status == HTTP_RECV_DATA) {

      if(r->transfer == HTTP_TRANSFER_CHUNKED && r->chunk_length == 0) {
        char length[64];
        if((ret = buffer_getline(&h->q.in, length, sizeof(length))) <= 0)
          break;

        scan_xlong(length, &r->chunk_length);
        continue;
      }

      if(r->content_length) {
        size_t a;
        
        if(r->content_length < (a = r->data.a - r->data.len)) a = r->content_length;
        if(a > 1024) a = 1024;

        stralloc_readyplus(&r->data, 1024);

        if((ret = buffer_get(&h->q.in, &r->data.s[r->data.len], r->data.a - r->data.len)) <= 0)
          break;

         putline("data", &r->data.s[r->data.len], 1, &h->q.in);

          r->data.len += ret;
          r->content_length -= ret;
          continue;
      }

      r->status = HTTP_STATUS_FINISH;
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
  }
}
