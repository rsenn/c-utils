#include "../tls_internal.h"

#ifdef HAVE_OPENSSL
#include <openssl/ssl.h>
#include <openssl/bio.h>

fd_type
tls_get_fd(tls_t* tls) {
  tls_bio_t* bio = SSL_get_rbio(tls);
  fd_type fd = 0;
  BIO_get_fd(bio, &fd);
  return fd;
}
#endif