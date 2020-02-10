#include "../http.h"
#include "../scan.h"
#include "../socket_internal.h"
#include "../str.h"
#include "../stralloc.h"
#include "../io.h"
#include "../byte.h"
#include <errno.h>
#include <assert.h>

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
      case HTTP_TRANSFER_UNDEF: break;
      case HTTP_TRANSFER_BOUNDARY: break;

      case HTTP_TRANSFER_CHUNKED: {
        if(r->ptr == r->chunk_length) {
          size_t skip;
          if((skip = scan_eolskip(&in->x[in->p], in->n - in->p))) {
            in->p += skip;
            r->chunk_length = 0;
          }
          // putnum("chunk end", r->ptr);
        }
        if(r->chunk_length == 0) {
          size_t i, bytes = in->n - in->p;
          if((i = byte_chr(&in->x[in->p], bytes, '\n')) < bytes) {
            i = scan_xlonglong(&in->x[in->p], &r->chunk_length);
            in->p += i;

            if((i = scan_eolskip(&in->x[in->p], in->n - in->p)))
              in->p += i;

            r->ptr = 0;
            if(r->chunk_length) {
              // putnum("chunk begin", r->chunk_length);
            } else {
              // putnum("transfer end", 0);
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
  // putnum("http_read_internal", r->status);
  return len;
}