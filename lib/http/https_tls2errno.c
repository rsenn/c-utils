#include "../http.h"
#include "../tls_internal.h"
#include <errno.h>

ssize_t
https_tls2errno(http* h, ssize_t ret) {
  int err;
  if(is_tls(h->sock))
    err = tls_errno(h->sock);
  errno = err;
  h->response->err = err;
  return err;
}
