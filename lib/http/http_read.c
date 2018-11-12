#include "../http.h"
#include "../scan.h"

size_t
http_read_internal(http* h, char* buf, size_t len) {
  buffer* in = &h->q.in;
  char* x = buffer_PEEK(in);
  char* y = buf + len;
  http_response* r;

  if((r = h->response) == NULL)
    return len;

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
}