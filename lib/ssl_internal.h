#ifndef SSL_INTERNAL_H
#define SSL_INTERNAL_H

#include "iarray.h"
#include "ssl.h"

#include <openssl/ssl.h>

typedef void want_func(fd_t);

typedef struct {
  SSL* ssl;
  int error;
  /* EVP_PKEY* key;
   X509* certificate;*/
  want_func *wantread, *wantwrite;
} ssl_instance;

extern const char *ssl_key, *ssl_certificate;
extern iarray ssl_list;

extern SSL_METHOD const *ssl_client_method, *ssl_server_method;
extern SSL_CTX *ssl_client_ctx, *ssl_server_ctx;

SSL_CTX* ssl_context(SSL_METHOD const* method);
ssl_instance* ssl_instance_get(fd_t);
ssl_instance* ssl_instance_new(fd_t);
ssize_t ssl_instance_return(ssl_instance* inst, int ret);

#endif
