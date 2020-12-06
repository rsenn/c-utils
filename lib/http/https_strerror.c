#include "../http.h"
#include "../tls.h"

const char*
https_strerror(http* h, int ret) {
  const char* err;

  err = tls_strerror(h->ssl, ret);

  return err;
}
