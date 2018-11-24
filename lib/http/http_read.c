#include "../http.h"
#include "../scan.h"
#include "../socket_internal.h"
#include "../str.h"
#include "../stralloc.h"
#include "../io.h"
#include "../byte.h"
#include <errno.h>
#include <assert.h>

size_t http_read_internal(http* h, char* buf, size_t len);

static void
putnum(const char* what, ssize_t n) {
  buffer_puts(buffer_1, what);
  buffer_puts(buffer_1, ": ");
  buffer_putint64(buffer_1, n);
  buffer_putnlflush(buffer_1);
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
  /*
  buffer_puts(buffer_1, " (bytes in recvb: ");
  buffer_putulong(buffer_1, buf->n - buf->p);
  buffer_puts(buffer_1, ")");*/
  buffer_putnlflush(buffer_1);
}

ssize_t
http_socket_read(fd_t fd, void* buf, size_t len, buffer* b) {
  int s;
  http* h = b->cookie;
  http_response* r = h->response;
  // s = winsock2errno(recv(fd, buf, len, 0));
  s = io_tryread(fd, buf, len);
  /*  buffer_puts(buffer_1, "io_tryread(");
    buffer_putlong(buffer_1, fd);
    buffer_puts(buffer_1, ", ");
    buffer_putptr(buffer_1, buf);
    buffer_puts(buffer_1, ", ");
    buffer_putulong(buffer_1, len);
    buffer_puts(buffer_1, ") = ");
    buffer_putlong(buffer_1, s);
    buffer_putnlflush(buffer_1);*/
  if(s == 0) {
    r->status = HTTP_STATUS_CLOSED;
  } else if(s == -1) {
    if(errno != EWOULDBLOCK && errno != EAGAIN) {
      r->status = HTTP_STATUS_ERROR;
    } else {
      errno = 0;
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
  ssize_t ret = 0, bytesread = 0;
  buffer* in = &h->q.in;
  while(r->status == HTTP_RECV_HEADER) {
    size_t bytesavail = in->n - in->p;
    h->q.in.op = NULL;
    if((ret = buffer_getline_sa(&h->q.in, &r->data)) <= 0)
      break;
    bytesread += bytesavail - (in->n - in->p);
    stralloc_trim(&r->data, "\r\n", 2);
    stralloc_nul(&r->data);
    //  putline("Header", r->data.s, -r->data.len, &h->q.in);
    if(r->data.len == 0) {
      r->ptr = in->p;
      r->status = HTTP_RECV_DATA;
      ret = 1;
      break;
    }
    putline("Header", r->data.s, -r->data.len, in);
    if(stralloc_startb(&r->data, "Content-Type: multipart", 23)) {
      size_t p = str_find(r->data.s, "boundary=");
      if(r->data.s[p]) {
        stralloc_copys(&r->boundary, &r->data.s[p + str_len("boundary=")]);
      }
      r->transfer = HTTP_TRANSFER_BOUNDARY;
    } else if(stralloc_startb(&r->data, "Content-Length:", 15)) {
      scan_uint64(&r->data.s[16], &r->content_length);
      r->transfer = HTTP_TRANSFER_LENGTH;
      putnum("content length", r->content_length);
    } else if(stralloc_starts(&r->data, "Transfer-Encoding:") && stralloc_contains(&r->data, "chunked")) {
      r->chunk_length = 0;
      r->transfer = HTTP_TRANSFER_CHUNKED;
    }
    stralloc_zero(&r->data);
  }
  h->q.in.op = (buffer_op_proto*)&http_socket_read;
  return ret;
}

/**
 * @brief http_read_internal
 * @param h       http* struct
 * @param buf     buffer we've just written to
 * @param len     number of bytes we've just written
 * @return
 */
size_t
http_read_internal(http* h, char* buf, size_t len) {
  buffer* in = &h->q.in;
  char* x = buffer_PEEK(in); /* buffer read pos */
  char* y = buf + len;       /* buffer write pos + len */
  http_response* r;
  if((r = h->response) == NULL)
    return len;
  if(r->status == HTTP_RECV_HEADER) {
    if(http_read_header(h, r) > 0)
      r->ptr = 0;
  }
  if(r->status == HTTP_RECV_DATA) {
    switch(r->transfer) {
      case HTTP_TRANSFER_CHUNKED: {
        if(r->ptr == r->chunk_length) {
          if(in->n - in->p >= 2) {
            if(in->x[in->p] == '\r')
              ++in->p;
            if(in->x[in->p] == '\n')
              ++in->p;
            r->chunk_length = 0;
          }
          putnum("chunk end", r->ptr);
        }
        if(r->chunk_length == 0) {
          size_t i, bytes = in->n - in->p;
          if((i = byte_chr(&in->x[in->p], bytes, '\n')) < bytes) {
            i = scan_xint64(&in->x[in->p], &r->chunk_length);
            in->p += i;
            if(in->x[in->p] == '\r')
              ++in->p;
            if(in->x[in->p] == '\n')
              ++in->p;
            r->ptr = 0;
            if(r->chunk_length) {
              putnum("chunk begin", r->chunk_length);
            } else {
              putnum("transfer end", 0);
              r->status = HTTP_STATUS_FINISH;
            }
          }
        }
        break;
      }
      case HTTP_TRANSFER_LENGTH: {
        if(r->ptr == r->content_length)
          r->status = HTTP_STATUS_FINISH;
        break;
      }
    }
  }
  return len;
}

ssize_t
http_read(http* h, char* buf, size_t len) {
  buffer* b = &h->q.in;
  ssize_t bytes, n, ret = 0;
  http_response* r;
  while(len) {
    bytes = b->n - b->p;
    if((n = buffer_freshen(b)) <= 0)
      break;
    if(b->n - b->p > bytes)
      putnum("growbuf", (b->n - b->p) - bytes);
    buffer_dump(buffer_1, b);
    if(h->response->status != HTTP_RECV_DATA)
      break;
    r = h->response;
    if(n + r->ptr > r->content_length)
      n = r->content_length - r->ptr;
    if(n >= (ssize_t)len)
      n = (ssize_t)len;
    byte_copy(buf, (size_t)n, b->x + b->p);
    putnum("skipbuf", n);
    len -= (size_t)n;
    buf += n;
    ret += n;
    b->p += (size_t)n;
    if(b->p == b->n)
      b->p = b->n = 0;
    // r->content_length -= n;
    r->ptr += n;
    putnum("ptr", r->ptr);
    if(r->ptr == r->content_length && b->n - b->p > 0) {
      http_read_internal(h, &b->x[b->p], b->n - b->p);

      if(r->status == HTTP_STATUS_FINISH) {
        io_dontwantread(h->sock);
        break;
      }
    }
  }
  return ret;
}