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
  /*

  buffer recvb;
  buffer_init(&recvb, (buffer_op_sys*)&buffer_dummyread, -1, r->body.s, r->body.len);
  recvb.p = r->ptr;

  for(;;) {
    ret = recv(h->sock, recvbuf, sizeof(recvbuf), 0);
    err = errno;

    if(ret == -1 && errno == EAGAIN) {
      errno = 0;
      break;
    }

    if(ret > 0) {
      stralloc_catb(&r->body, recvbuf, ret);
      recvb.a = recvb.n = r->body.len;
      recvb.x = r->body.s;
    }
    if(ret == 0)
      r->status = HTTP_STATUS_CLOSED;
    else if(err != 0)
      r->statu
      s = HTTP_STATUS_ERROR;
    break;
  }

  for(;;) {
    char line[1024];
    size_t sptr = r->ptr;
    ret = buffer_getline(&recvb, line, sizeof(line) - 1);

    if(ret == 0 && line[0] == '\0') {
      //   putline("Again", line, 0, &recvb);
      return;
    }

    //  putline("Line", line, 0, &recvb);

    r->ptr = recvb.p;
    r->line++;

    if(ret >= 0) {
      unsigned long n, p;
      while(ret > 0 && is_space(line[ret - 1])) ret--;
      line[ret] = '\0';

      if(r->status < HTTP_RECV_DATA && str_chr(line, ':') < ret) {
        r->status = HTTP_RECV_HEADER;

      }
        if(r->status == HTTP_RECV_HEADER) r->status = HTTP_RECV_DATA;
      }

      if(r->status == HTTP_RECV_DATA && r->boundary.len) {
        stralloc_zero(&r->data);

        if(!buffer_get_token_sa_pred(&recvb, &r->data, boundary_predicate, &r->boundary)) {
          putline("Boundary", r->data.s, r->data.len, &recvb);
        }
      } else if(r->status == HTTP_RECV_DATA) {

        if(n == 0) {
          r->status = HTTP_STATUS_FINISH;
          return;
        }

      if(recvb.n - recvb.p >= n) {
                  stralloc_readyplus(&r->data, n);
                  buffer_get(&recvb, &r->data.s[r->data.len], n);
                  r->data.len += n;
                  buffer_puts(buffer_2, "data len=");
                  buffer_putulong(buffer_2, r->data.len);
                  buffer_putnlflush(buffer_2);
      }
        if(r->content_length) {

        } else if((p = scan_xlong(line, &n)) > 0) {
          r->chnk++;

        }

        n = buffer_getline(&recvb, line, sizeof(line));
        // putline("Newline", "", -n, &recvb);
        if(recvb.n - recvb.p <= 0) return;
        continue;
      }

      if(!r->status) {
        putline("Response", line, ret, &recvb);
      }
    }
  }*/
}
