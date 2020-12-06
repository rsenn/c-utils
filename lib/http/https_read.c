#include "../http.h"
#include "../tls.h"
#include <assert.h>

ssize_t
https_read(fd_t fd, void* buf, size_t len, void* b) {
  ssize_t ret = 0;
  http* h = ((buffer*)b)->cookie;
  assert(h->tls);
  assert(h->connected);
  ret = tls_read(fd, buf, len);

#if DEBUG_HTTP
  buffer_putspad(buffer_2, "https_read ", 18);
  buffer_puts(buffer_2, "sock=");
  buffer_putlong(buffer_2, h->sock);
  buffer_puts(buffer_2, " ret=");
  buffer_putlong(buffer_2, ret);
  if(ret <= 0) {
    buffer_puts(buffer_2, " err=");
    buffer_puts(buffer_2, https_strerror(h, ret)));
  }
  buffer_putnlflush(buffer_2);
#endif
  return ret;
}
