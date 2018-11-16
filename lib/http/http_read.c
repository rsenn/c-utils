#include "../http.h"
#include "../scan.h"
#include "../socket_internal.h"
#include "../str.h"
#include "../stralloc.h"
#include "../io.h"
#include "../byte.h"
#include <errno.h>

size_t http_read_internal(http* h, char* buf, size_t len);

ssize_t
http_socket_read(fd_t fd, void* buf, size_t len, buffer* b) {
  int s;
  http* h = b->cookie;
  http_response* r = h->response;

  // s = winsock2errno(recv(fd, buf, len, 0));
  s = io_tryread(fd, buf, len);

  buffer_puts(buffer_2, "io_tryread(");
  buffer_putlong(buffer_2, fd);
  buffer_puts(buffer_2, ", ");
  buffer_putptr(buffer_2, buf);
  buffer_puts(buffer_2, ", ");
  buffer_putulong(buffer_2, len);
  buffer_puts(buffer_2, ") = ");
  buffer_putlong(buffer_2, s);
  buffer_putnlflush(buffer_2);

  if(s == 0) {
    r->status = HTTP_STATUS_CLOSED;
  } else if(s == -1) {

    if(errno != EWOULDBLOCK && errno != EAGAIN) {
      r->status = HTTP_STATUS_ERROR;
    }
  }

  if(s > 0) {
    size_t n = h->q.in.n;
    h->q.in.n += s;
    s = http_read_internal(h, buf, s);
    h->q.in.n = n;
  }

  return s;
}

ssize_t
http_read_header(http* h, http_response* r) {
  ssize_t ret = 0;

  while(r->status == HTTP_RECV_HEADER) {
    h->q.in.op = NULL;

    if((ret = buffer_getline_sa(&h->q.in, &r->data)) <= 0)
      break;

    stralloc_trim(&r->data, "\r\n", 2);
    stralloc_nul(&r->data);

    //  putline("Header", r->data.s, -r->data.len, &h->q.in);

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
      scan_ulong(&r->data.s[16], &r->content_length);
      r->transfer = HTTP_TRANSFER_LENGTH;
    } else {
      r->transfer = HTTP_TRANSFER_CHUNKED;
    }

    stralloc_zero(&r->data);
  }
  h->q.in.op = &http_socket_read;
}

size_t
http_read_internal(http* h, char* buf, size_t len) {
  buffer* in = &h->q.in;
  char* x = buffer_PEEK(in);
  char* y = buf + len;
  http_response* r;

  if((r = h->response) == NULL)
    return len;

  if(r->status == HTTP_RECV_HEADER) {
    http_read_header(h, r);
  }

  if(r->status == HTTP_RECV_DATA) {
    if(r->transfer == HTTP_TRANSFER_CHUNKED && r->chunk_length == 0) {
      if(y - x > 0) {
        size_t i = scan_xlong(x, &r->chunk_length);
        if(x[i] == '\r')
          ++i;
        if(x[i] == '\n')
          ++i;
        in->p += i;
      }
    }
    if(len > 0) {
      r->ptr += len;
      switch(r->transfer) {
        case HTTP_TRANSFER_LENGTH: {
          if(r->ptr == r->content_length)
            r->status = HTTP_STATUS_FINISH;
          break;
        }
        case HTTP_TRANSFER_CHUNKED: {
          if(r->ptr == r->chunk_length) {
            r->status = HTTP_RECV_DATA;
            r->chunk_length = 0;
          }
          break;
        }
      }
    }
  }
  return len;
}

ssize_t
http_read(http* h, char* buf, size_t len) {
  buffer* b = &h->q.in;
  ssize_t n, r = 0;
  while(len) {
    if((n = buffer_freshen(b)) < 0)
      return n;
    if(n == 0)
      break;
    if(n >= (ssize_t)len)
      n = (ssize_t)len;
    byte_copy(buf, (size_t)n, b->x + b->p);
    b->p += (size_t)n;
    len -= (size_t)n;
    buf += n;
    r += n;
  }
  return r;
}
