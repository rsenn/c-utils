#include "../http.h"
#include "../scan.h"
#include "../socket_internal.h"
#include <errno.h>

ssize_t
http_read_header(http* h, http_response* r) {
  ssize_t ret = 0;

  while(r->status == HTTP_RECV_HEADER) {
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
