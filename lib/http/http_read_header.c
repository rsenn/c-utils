#include "../http.h"
#include "../scan.h"
#include "../socket_internal.h"
#include "../str.h"
#include "../stralloc.h"
#include "../io.h"
#include "../byte.h"
#include "../case.h"
#include <errno.h>
#include <assert.h>

ssize_t http_socket_read(fd_type fd, void* buf, size_t len, void* b);

static inline void
putline(const char* what, const char* b, ssize_t l, int i) {
  buffer_puts(buffer_2, what);
  buffer_puts(buffer_2, "[");
  buffer_putulong(buffer_2, i);
  buffer_puts(buffer_2, "]");
  buffer_puts(buffer_2, ": ");

  if(l <= 0)
    buffer_puts(buffer_2, b);
  else
    while(l-- > 0)
      buffer_put(buffer_2, b++, 1);

  buffer_putnlflush(buffer_2);
}

ssize_t
http_read_header(buffer* in, stralloc* out, http_response* response) {
  ssize_t ret = 0, bytesread = 0;

  while(response->status == HTTP_RECV_HEADER) {
    size_t len, bytesavail = buffer_LEN(in), start = out->len;
    char* buf;

    if((ret = buffer_getline_sa(in, out)) <= 0)
      break;

    bytesread += bytesavail - (buffer_LEN(in));
    stralloc_nul(out);
    buf = &out->s[start];

    if((len = byte_trimr(buf, out->len - start, "\r\n", 2)) == 0) {
      response->status = HTTP_RECV_DATA;
      ret = 1;
      break;
    }

#ifdef DEBUG_HTTP
    putline("Header", buf, len, byte_count(out->s, out->len, '\n'));
#endif

    if(response->code == -1) {
      if(str_start(out->s, "HTTP")) {
        unsigned int code;

        size_t p = scan_nonwhitenskip(out->s, out->len);
        p += scan_whitenskip(&out->s[p], out->len - p);

        if(scan_uint(&out->s[p], &code) > 0)
          response->code = code;
      }
    }

    if(out->len - start >= 23 && !case_diffb(buf, 23, "Content-Type: multipart")) {
      size_t p = str_find(buf, "boundary=");

      if(buf[p])
        stralloc_copys(&response->boundary, &buf[p + str_len("boundary=")]);

      response->transfer = HTTP_TRANSFER_BOUNDARY;
      // response->headers_len = 0;
    } else if(out->len - start >= 15 && !case_diffb(buf, 15, "Content-Length:")) {
      scan_ulonglong(&buf[16], &response->content_length);
      response->transfer = HTTP_TRANSFER_LENGTH;
      // response->headers_len = 0;
    } else if(out->len - start >= 18 && !case_diffb(buf, 18, "Transfer-Encoding:") && str_contains(buf, "chunked")) {
      // response->headers_len = 0;
      response->chunk_length = 0;
      response->content_length = 0;
      response->transfer = HTTP_TRANSFER_CHUNKED;
    }
  }

#ifdef DEBUG_HTTP_
  buffer_putspad(buffer_2, "\x1b[1;33mhttp_read_header\x1b[0m", 30);

  buffer_puts(buffer_2, " bytesread=");
  buffer_putlong(buffer_2, bytesread);
  buffer_puts(buffer_2, " ret=");
  buffer_putlong(buffer_2, ret);
#endif

  return ret > 0 && bytesread > 0 ? bytesread : ret;
}
