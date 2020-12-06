#include "../http.h"
#include "../tls.h"
#include <assert.h>

ssize_t
https_connect(http* h) {
  ssize_t ret;
  char* msg = 0;
  assert(!h->connected);
  ret = tls_connect(h->sock);

  if(ret == 1) {
    h->connected = 1;
#if DEBUG_HTTP
    buffer_putsflush(buffer_2, "https_connect done!\n");
#endif
  }
#if DEBUG_HTTP
  buffer_putspad(buffer_2, "https_connect ", 18);
  buffer_puts(buffer_2, "sock=");
  buffer_putlong(buffer_2, h->sock);
  buffer_puts(buffer_2, " ret=");
  buffer_putlong(buffer_2, ret);

  if(errno) {
    buffer_puts(buffer_2, " errno=");
    buffer_putlong(buffer_2, errno);
  }
  buffer_puts(buffer_2, " err=");
  buffer_puts(buffer_2, https_strerror(h, ret));
  buffer_putnlflush(buffer_2);

#endif
  return ret;
}