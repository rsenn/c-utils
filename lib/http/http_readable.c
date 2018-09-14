#include "../buffer.h"
#include "../byte.h"
#include "../http.h"
#include "../io_internal.h"
#include "../scan.h"
#include "../socket.h"
#include "../str.h"
#include "../windoze.h"
#include <ctype.h>
#include <errno.h>

#define is_space(c) ((c) == ' ' || (c) == '\t' || (c) == '\r' || (c) == '\n')

ssize_t
buffer_dummyread() {
  return 0;
}

static void

putline(const char* what, const char* b, ssize_t l, buffer* buf) {
  buffer_puts(buffer_1, what);
  buffer_puts(buffer_1, "[");
  buffer_putulong(buffer_1, l <= 0 ? -l : l);
  buffer_puts(buffer_1, "]");
  buffer_puts(buffer_1, ": ");
  if(l <= 0)
    buffer_puts(buffer_1, b);
  else {
    while(l-- > 0) buffer_put(buffer_1, b++, 1);
  }
  buffer_puts(buffer_1, " (bytes in recvb: ");
  buffer_putulong(buffer_1, buf->n - buf->p);
  buffer_puts(buffer_1, ")");
  buffer_putnlflush(buffer_1);
}

static int
boundary_predicate(stralloc* sa, void* arg) {
  stralloc* pred = arg;

  if(pred->len >= sa->len) {
    if(!byte_diff(&sa->s[sa->len - pred->len], pred->len, pred->s)) return 1;
  }
  return 0;
}

void
http_readable(http* h) {
  ssize_t ret;
  size_t lines = 0;

  if(h->response) {
    char recvbuf[8192];
    int err;
    http_response* r = h->response;
    buffer recvb = BUFFER_INIT(buffer_dummyread, -1, r->body.s, r->body.len);
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
        r->status = CLOSED;
      else if(err != 0)
        r->status = ERR;
      break;
    }

    for(;;) {
      char line[1024];
      size_t sptr = r->ptr;
      ret = buffer_getline(&recvb, line, sizeof(line));

      if(ret == 0 && line[0] == '\0') {
        putline("Again", line, 0, &recvb);
        return;
      }

      putline("Line", line, 0, &recvb);

      r->ptr = recvb.p;
      r->line++;

      if(ret >= 0) {
        unsigned long n, p;
        while(ret > 0 && is_space(line[ret - 1])) ret--;
        line[ret] = '\0';

        if(r->part < CHUNKS && line[str_chr(line, ':')] == ':') {
          /*  if(r->part == HEADER)*/ putline("Header", line, ret, &recvb);
          r->part = HEADER;

          if(!str_diffn(line, "Content-Type: multipart", 23)) {
            static const char* const boundstr = "boundary=";
            p = str_find(line, boundstr);

            if(line[p]) {
              stralloc_copys(&r->boundary, &line[p + str_len(boundstr)]);
            }
          }
        } else {
          if(r->part == HEADER) r->part = CHUNKS;
        }

        if(r->part == CHUNKS && r->boundary.len) {
          stralloc_zero(&r->data);

          if(!buffer_get_token_sa_pred(&recvb, &r->data, boundary_predicate, &r->boundary)) {
            putline("Boundary", r->data.s, r->data.len, &recvb);
          }
        } else if(r->part == CHUNKS && (p = scan_xlong(line, &n)) > 0) {
          ssize_t n;

          if(n == 0) {
            r->status = DONE;
            return;
          } else if(recvb.n - recvb.p >= n) {
            stralloc_readyplus(&r->data, n);
            buffer_get(&recvb, &r->data.s[r->data.len], n);
            r->data.len += n;
            buffer_puts(buffer_1, "data len=");
            buffer_putulong(buffer_1, r->data.len);
            buffer_putnlflush(buffer_1);
            r->chnk++;
          } else {
            r->ptr = sptr;
            return; /* goto again; */
          }
          n = buffer_getline(&recvb, line, sizeof(line));
          putline("Newline", "", -n, &recvb);
          if(recvb.n - recvb.p <= 0) return;
          continue;
        }

        if(!r->part) {
          putline("Response", line, ret, &recvb);
        }
      }
    }
  }
}
