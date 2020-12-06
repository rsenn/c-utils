#include "../http.h"
#include "../tls.h"
#include <errno.h>
#include <string.h>

const char*
http_strerror(http* h, int ret) {
  const char* err = "success";

  if(ret == -1) {
    if(h->tls)
      err = tls_strerror(h->sock, ret);
    else
      err = strerror(errno);
  }
  return err;
}
