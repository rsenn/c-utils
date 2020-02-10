#include "../http.h"
#include "../scan.h"
#include "../socket_internal.h"
#include "../str.h"
#include "../stralloc.h"
#include "../io.h"
#include "../byte.h"
#include <errno.h>
#include <assert.h>



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
  /*
  buffer_puts(buffer_2, " (bytes in recvb: ");
  buffer_putulong(buffer_2, buf->n - buf->p);
  buffer_puts(buffer_2, ")");*/
  buffer_putnlflush(buffer_2);
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
    stralloc_trimr(&r->data, "\r\n", 2);
    stralloc_nul(&r->data);
    //  putline("Header", r->data.s, -r->data.len, &h->q.in);
    // putnum("data.len", r->data.len);
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
      scan_ulonglong(&r->data.s[16], &r->content_length);
      r->transfer = HTTP_TRANSFER_LENGTH;
      // putnum("content length", r->content_length);
    } else if(stralloc_starts(&r->data, "Transfer-Encoding:") && stralloc_contains(&r->data, "chunked")) {
      r->chunk_length = 0;
      r->transfer = HTTP_TRANSFER_CHUNKED;
    }
    stralloc_zero(&r->data);
  }
  // putnum("http_read_header", r->status);
  h->q.in.op = (buffer_op_proto*)&http_socket_read;
  return ret;
}