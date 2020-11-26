#ifndef SSL_INTERNAL_H
#define SSL_INTERNAL_H

#include "iarray.h"
#include "ssl.h"

#include <openssl/ssl.h>
#include <openssl/x509.h>

typedef void want_func(fd_t);

typedef struct {
  ssl_t* ssl;
  int error;
  /* EVP_PKEY* key;
   X509* certificate;*/
  want_func *wantread, *wantwrite;
  char errstr[32];
} ssl_instance;

extern const char *ssl_key, *ssl_certificate;
extern iarray ssl_list;

extern ssl_method_t const *ssl_client_method, *ssl_server_method;
extern ssl_ctx_t *ssl_client_ctx, *ssl_server_ctx;

ssl_ctx_t* ssl_context(ssl_method_t const* method);
ssl_instance* ssl_instance_get(fd_t fd);
ssl_instance* ssl_instance_new(fd_t fd);
ssize_t ssl_instance_return(ssl_instance* i, int ret);
ssl_ctx_t* ssl_context(ssl_method_t const* method);
const char* ssl_instance_error(ssl_instance* i);
int ssl_instance_errno(ssl_instance* i);
void ssl_instance_debug(ssl_instance* i);
int ssl_instance_handshake(ssl_instance* i);

#endif
