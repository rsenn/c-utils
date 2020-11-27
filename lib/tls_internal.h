#ifndef TLS_INTERNAL_H
#define TLS_INTERNAL_H

#include "iarray.h"
#include "tls.h"

#ifdef HAVE_OPENSSL
#include <openssl/ssl.h>
#include <openssl/x509.h>

typedef void want_func(fd_t);

typedef struct {
  tls_t* ssl;
  int error;
  /* EVP_PKEY* key;
   X509* certificate;*/
  want_func *wantread, *wantwrite;
  char errstr[32];
} tls_instance;

extern const char *tls_key, *tls_certificate;
extern iarray tls_list;

extern tls_method_t const *tls_client_method, *tls_server_method;
extern tls_ctx_t *tls_client_ctx, *tls_server_ctx;

tls_ctx_t* tls_context(tls_method_t const* method);
tls_instance* tls_instance_get(fd_t fd);
tls_instance* tls_instance_new(fd_t fd);
ssize_t tls_instance_return(tls_instance* i, int ret);
tls_ctx_t* tls_context(tls_method_t const* method);
const char* tls_instance_error(tls_instance* i);
int tls_instance_errno(tls_instance* i);
void tls_instance_debug(tls_instance* i);
int tls_instance_handshake(tls_instance* i);
#endif
#endif
