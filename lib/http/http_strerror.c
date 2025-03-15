#include "../http.h"
#include "../tls.h"
#include <errno.h>
#include <string.h>

const char*
http_strerror(http* h, int ret) {
  const char* err = "success";

  if(ret == -1)
    err = h->tls ? tls_strerror(h->sock) : strerror(errno);

  return err;
}
