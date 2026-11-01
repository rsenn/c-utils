#include "../http.h"
#include "../buffer.h"
#include "../io.h"
#include <errno.h>

/* Writes what http_body, http_chunk and http_chunk_end queued, as far as the
 * socket takes it. Returns 1 when everything is out, 0 when the socket is
 * full and it has to be called again when it is writable, -1 on an error.
 * The data is written straight to the socket since buffer_flush forgets what
 * a non blocking socket did not take. */
ssize_t
http_body_flush(http* h) {
  http_request* req = h->request;

  if(req == NULL)
    return 1;

  /* the headers go first */
  if(!h->sent)
    return 0;

  while(req->body_pos < req->body.len) {
    ssize_t w = http_socket_write(h->sock, req->body.s + req->body_pos, req->body.len - req->body_pos, &h->q.out);

    if(w < 0) {
      if(errno == EAGAIN || errno == EWOULDBLOCK) {
        io_wantwrite(h->sock);
        return 0;
      }
      return -1;
    }

    if(w == 0) {
      errno = EPIPE;
      return -1;
    }

    req->body_pos += (size_t)w;
  }

  /* all of it is out, start over so a long stream does not pile up */
  req->body.len = 0;
  req->body_pos = 0;

  io_dontwantwrite(h->sock);

  return 1;
}
